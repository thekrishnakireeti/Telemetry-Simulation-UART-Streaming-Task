#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

unsigned char calculate_checksum(const char *str) {
    unsigned char chk = 0;
    for(int i=0; str[i] != '\0'; i++) {
        chk ^= str[i];
    }
    return chk;
}

int main() {
    int uart_fd = open("/dev/pts/2", O_WRONLY | O_NOCTTY);
    if (uart_fd < 0) {
        perror("Failed to open UART");
        return 1;
    }

    struct termios tty;
    tcgetattr(uart_fd, &tty);
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    tcsetattr(uart_fd, TCSANOW, &tty);

    srand(time(NULL));

    while(1) {
        long timestamp = (long)time(NULL) * 1000; // ms
        float ax = ((rand()%200)-100)/100.0;
        float ay = ((rand()%200)-100)/100.0;
        float az = ((rand()%200)-100)/100.0;
        float gx = ((rand()%200)-100)/100.0;
        float gy = ((rand()%200)-100)/100.0;
        float gz = ((rand()%200)-100)/100.0;
        float alt = (rand()%1000)/10.0;
        float temp = 25 + (rand()%100)/10.0;

        char frame[256];
        snprintf(frame, sizeof(frame),
                 "L1,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
                 timestamp, ax, ay, az, gx, gy, gz, alt, temp);

        unsigned char chk = calculate_checksum(frame);

        char final[300];
        snprintf(final, sizeof(final), "$%s*%02X\n", frame, chk);

        write(uart_fd, final, strlen(final));
        printf("Sent: %s", final);
        usleep(50000); // 50 ms → 20 Hz
    }

    close(uart_fd);
    return 0;
}

