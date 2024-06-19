// Barktree

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>

#define SQR(x) ((x) * (x))
#define SIGN(x) (((x) > 1e-10) - ((x) < -1e-10))

const double PI_CONST = acos(-1);

using namespace std;

bool isInsideTriangle(
    double px, double py,
    double ax, double ay, double bx, double by, double cx, double cy
) {
    int d1 = SIGN(((px - ax) * (by - ay) - (py - ay) * (bx - ax)) / sqrt(SQR(by - ay) + SQR(bx - ax)));
    int d2 = SIGN(((px - bx) * (cy - by) - (py - by) * (cx - bx)) / sqrt(SQR(cy - by) + SQR(cx - bx)));
    int d3 = SIGN(((px - cx) * (ay - cy) - (py - cy) * (ax - cx)) / sqrt(SQR(ay - cy) + SQR(ax - cx)));

    return d1 && d1 == d2 && d2 == d3;
}

double relativeDotProduct(
    double px, double py,
    double ax, double ay, double bx, double by
) {
    return ((ax - px) * (bx - px) + (ay - py) * (by - py))
        / sqrt(SQR(bx - px) + SQR(by - py));
}

double computeAngle(
    double px, double py,
    double ax, double ay, double bx, double by
) {
    double result = atan2(by - py, bx - px) - atan2(ay - py, ax - px);

    if (result < PI_CONST) result += 2 * PI_CONST;
    if (result > PI_CONST) result -= 2 * PI_CONST;

    return result;
}

void findIntersection(
    double &ix, double &iy,
    double px1, double py1, double px2, double py2,
    double qx1, double qy1, double qx2, double qy2
) {
    double p_dx = px2 - px1;
    double p_dy = py2 - py1;
    double q_dx = qx2 - qx1;
    double q_dy = qy2 - qy1;

    double p_cross_qy = p_dx * q_dy;
    double p_cross_qx = p_dy * q_dx;

    ix = (py1 * p_dx * q_dx - px1 * p_dy * q_dx - qy1 * p_dx * q_dx + qx1 * p_cross_qy) / (p_cross_qy - p_cross_qx);
    iy = (px2 * p_dy * q_dy - py2 * p_dx * q_dy - qx2 * p_dy * q_dy + qy2 * p_cross_qx) / (p_cross_qx - p_cross_qy);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(10);

    int n, m;
    cin >> n >> m;

    double nx[50], ny[50], mx[50], my[50];
    for (int i = 0; i < n; ++i) {
        cin >> nx[i] >> ny[i];
    }

    for (int i = 0; i < m; ++i) {
        cin >> mx[i] >> my[i];
    }

    int k = 0;
    double path_x[2501], path_y[2501], path_angle[2501], path_length[2501], max_length = 0;

    double current_x = 0, current_y = 0;
    for (int i = 0; i < n; ++i) {
        while (true) {
            int best_index = -1;
            double best_dot_product = -INFINITY;

            for (int j = 0; j < m; ++j) {
                if (!isInsideTriangle(
                    mx[j], my[j],
                    path_x[k], path_y[k], current_x, current_y, nx[i], ny[i]
                )) continue;

                double dot_product = relativeDotProduct(
                    path_x[k], path_y[k],
                    current_x, current_y, mx[j], my[j]
                );

                if (best_dot_product < dot_product) {
                    best_index = j;
                    best_dot_product = dot_product;
                }
            }

            double intersection_x, intersection_y;
            if (best_index >= 0) {
                findIntersection(
                    intersection_x, intersection_y,
                    path_x[k], path_y[k], mx[best_index], my[best_index],
                    current_x, current_y, nx[i], ny[i]
                );
            } else {
                intersection_x = nx[i];
                intersection_y = ny[i];
            }

            double angle_rotation = computeAngle(
                path_x[k], path_y[k],
                current_x, current_y, intersection_x, intersection_y
            );

            if (k > 0 && SIGN(path_angle[k]) != SIGN(path_angle[k] + angle_rotation)) {
                findIntersection(
                    intersection_x, intersection_y,
                    path_x[k - 1], path_y[k - 1], path_x[k], path_y[k],
                    current_x, current_y, nx[i], ny[i]
                );

                k -= 1;

                current_x = intersection_x;
                current_y = intersection_y;
            } else {
                path_angle[k] += angle_rotation;

                if (best_index >= 0) {
                    k += 1;
                    path_x[k] = mx[best_index];
                    path_y[k] = my[best_index];
                    path_angle[k] = SIGN(angle_rotation) * 1e-6;
                    path_length[k] = path_length[k - 1] + sqrt(SQR(path_x[k] - path_x[k - 1]) + SQR(path_y[k] - path_y[k - 1]));

                    current_x = intersection_x;
                    current_y = intersection_y;
                } else {
                    max_length = max(max_length, path_length[k] + sqrt(SQR(nx[i] - path_x[k]) + SQR(ny[i] - path_y[k])));

                    current_x = nx[i];
                    current_y = ny[i];

                    break;
                }
            }
        }
    }

    printf("%.2f\n", max_length);

    return 0;
}
