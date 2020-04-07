// Include Files
#include "decide.h"
#include <cstdlib>
#include <time.h>

void new_cars(double plaza[500], double v[500], double probc, const double
              probv[2], const double probw[3], const double VTypes[2], double
              vmax[500], double ways[500]) {
    int lanes, i, k;
    double tmp, temp;
    unsigned char exitg1;

    memset(&vmax[0], 0, 500U * sizeof(double));
    memset(&ways[0], 0, 500U * sizeof(double));
    srand(time(NULL));
    for (lanes = 0; lanes < 3; lanes++) {
        for (i = 0; i < 100; i++) {
            double my_rand = rand() % 100 * 0.01;
            if (my_rand <= probc) {
                // 在该位置随机产生一个车子
                tmp = my_rand;
                plaza[i + 100 * (lanes + 1)] = 1.0;
                temp = rand() % 100 * 0.01;
                if (temp <= probw[0]) {
                    ways[i + 100 * (lanes + 1)] = 1.0;
                }

                if ((temp > probw[0]) && (temp <= probw[0] + probw[1])) {
                    ways[i + 100 * (lanes + 1)] = 2.0;
                }

                if (temp > probw[0] + probw[1]) {
                    ways[i + 100 * (lanes + 1)] = 3.0;
                }

                k = 0;
                exitg1 = false;
                while ((!exitg1) && (k < 2)) {
                    // 随机生成一个车子应该有的最大车速
                    if (tmp <= probv[k]) {
                        vmax[i + 100 * (lanes + 1)] = VTypes[k];

                        // 判断属于哪个挡的车速并赋值
                        tmp = my_rand * vmax[i + 100 * (lanes + 1)];
                        v[i + 100 * (lanes + 1)] = std::ceil(tmp);

                        // 对当前位置随机赋予一个初速度
                        exitg1 = true;
                    } else {
                        k++;
                    }
                }
            }
        }
    }
}

void allot(const double plaza[4][500], double timeRow,
           double *rowTime, double *colTime) {
    double row, col;
    int lanes;
    unsigned char x[76];
    int k;
    double temp;
    row = col = 1.0;
    for (lanes = 0; lanes < 3; lanes++) {
        for (k = 0; k < 76; k++) {
            x[k] = (plaza[0][(k + 100 * (lanes + 1)) + 24] == 1.0);
        }

        temp = x[0];
        for (k = 0; k < 75; k++) {
            temp += (double)x[k + 1];
        }

        row += temp;
        for (k = 0; k < 76; k++) {
            x[k] = (plaza[1][(k + 100 * (lanes + 1)) + 24] == 1.0);
        }

        temp = x[0];
        for (k = 0; k < 75; k++) {
            temp += (double)x[k + 1];
        }

        row += temp;
        for (k = 0; k < 76; k++) {
            x[k] = (plaza[2][(k + 100 * (lanes + 1)) + 24] == 1.0);
        }

        temp = x[0];
        for (k = 0; k < 75; k++) {
            temp += (double)x[k + 1];
        }

        col += temp;
        for (k = 0; k < 76; k++) {
            x[k] = (plaza[3][(k + 100 * (lanes + 1)) + 24] == 1.0);
        }

        temp = x[0];
        for (k = 0; k < 75; k++) {
            temp += (double)x[k + 1];
        }

        col += temp;
    }

    *rowTime = timeRow * row / (row + col);
    *colTime = timeRow * col / (row + col);
}

void simulation_and_decision(double probc[4], const double probv[4][2], const double probw[4][3],
                             const double VTypes[4][2], double *rowTime, double *colTime) {

    double plaza[4][500], v[4][500], vmax[4][500];
    double ways[4][500];

    for (int i = 0; i < 500; i++)
        for (int j = 0; j < 4; j++) {
            if (i < 100 || i >= 400)
                plaza[j][i] = -1;
            else
                plaza[j][i] = 0;
            v[j][i] = 0;
            ways[j][i] = 0;
        }

    for (int i = 0; i < 4; i++) {
        new_cars(plaza[i], v[i], probc[i], probv[i], probw[i], VTypes[i], vmax[i], ways[i]);
    }

    allot(plaza, 100.0, rowTime, colTime);
}

void begin_decide(int car_number[4], double &rowTime, double &colTime) {
    double probc[4];
    double probv[4][2];
    double probw[4][3];
    double VTypes[4][2];
    for (int i = 0; i < 4; i++) {
        probc[i] = car_number[i] * 4.5 / 300;
        probv[i][0] = 0.1;
        probv[i][1] = 1;
        probw[i][0] = 0.3;
        probw[i][1] = 0.2;
        probw[i][2] = 0.5;
        VTypes[i][0] = 1;
        VTypes[i][1] = 2;
    }

    simulation_and_decision(probc, probv, probw, VTypes, &rowTime, &colTime);
}
