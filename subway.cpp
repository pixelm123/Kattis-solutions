#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long double ld;
const ld EPSILON = 1e-9;

struct Point {
    ld x, y;
    Point() { x = y = 0; }
    Point(ld _x, ld _y) : x(_x), y(_y) {}

    // Compare x-coordinate, if equal compare y-coordinate
    bool operator<(const Point &p) const {
        if (fabs(x - p.x) > EPSILON) return x < p.x;
        return y < p.y;
    }

    // Compare both x and y
    bool operator==(const Point &p) const { return (fabs(x - p.x) < EPSILON && fabs(y - p.y) < EPSILON); }

    // Arithmetic Operations (Translation and Scaling)
    Point operator+(const Point &p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
    Point operator*(const ld &r) const { return Point(x * r, y * r); }
    Point operator/(const ld &r) const { return Point(x / r, y / r); }
};

// Euclidean Distance
ld distance(Point p1, Point p2) { return hypot(p1.x - p2.x, p1.y - p2.y); }

// Output Representation of a point
ostream &operator<<(ostream &os, const Point &p) { return os << "(" << p.x << "," << p.y << ")"; }

struct Line {
    ld a, b, c;  // ax + by + c = 0, b = 0 for all vertical lines and 1 otherwise
    Line(ld _a, ld _b, ld _c) : a(_a), b(_b), c(_c) {}  // Direct Construction

    Line(Point p1, Point p2) {  // Construct from two points
        if (fabs(p1.x - p2.x) < EPSILON) {  // Vertical Line
            a = 1.0;
            b = 0.0;
            c = -p1.x;  // Negative of x-intercept
        } else {
            a = -(p1.y - p2.y) / (p1.x - p2.x);  // Negative Gradient
            b = 1.0;                             // Non-vertical line
            c = -(a * p1.x) - p1.y;              // Negative of y-intercept
        }
    }

    Line(Point p, ld m) {  // Construct from point and slope
        if (fabs(m) < EPSILON) {  // Vertical Line
            a = 1.0;
            b = 0.0;
            c = -p.x;  // Negative of x-intercept
        } else {
            a = -m;
            b = 1.0;
            c = -(a * p.x) - p.y;  // Negative of y-intercept
        }
    }

    Line() {
        a = b = c = 0;  // No line
    }
};

struct Vector {
    ld x, y;
    Vector(ld _x, ld _y) : x(_x), y(_y) {}
    Vector(Point p1, Point p2) : x(p2.x - p1.x), y(p2.y - p1.y) {}

    // Vector Operations
    Vector operator+(const Vector &v) const { return Vector(x + v.x, y + v.y); }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y); }
    Vector operator*(const ld &r) const { return Vector(x * r, y * r); }
    Vector operator/(const ld &r) const { return Vector(x / r, y / r); }

    // Length
    ld length() { return sqrt(x * x + y * y); }

    // Length Square
    ld length_sq() { return x * x + y * y; }
};

// Vector Products
ld dot(Vector v1, Vector v2) { return v1.x * v2.x + v1.y * v2.y; }
ld cross(Vector v1, Vector v2) { return v1.x * v2.y - v1.y * v2.x; }

// Calculate angle between BA and BC (centered at B)
ld angle(const Point &a, const Point &b, const Point &c) {
    Vector v1 = Vector(b, a);
    Vector v2 = Vector(b, c);
    return acos(dot(v1, v2) / (v1.length() * v2.length()));
}

// Counter Clockwise Test and Variants
bool ccw(Point p1, Point p2, Point p3) { return cross(Vector(p1, p2), Vector(p1, p3)) > EPSILON; }

// Minimum Length from point to line, and point on line
ld distToLine(Point p, Line l, Point &closest_pt) {
    ld t = (l.a * p.x + l.b * p.y + l.c) / (l.a * l.a + l.b * l.b);
    closest_pt = Point(p.x - l.a * t, p.y - l.b * t);
    return hypot(p.x - closest_pt.x, p.y - closest_pt.y);
}

// The angle going from A to B to C in a clockwise direction [0, 2pi)
ld corrected_angle(Point a, Point b, Point c) {
    ld anglex = angle(a, b, c);
    if (ccw(a, b, c)) anglex = 2 * M_PI - anglex;
    return anglex;
}

int num_points, distance_limit;
Point origin_point(0, 0);
Point one_zero(1, 0);
vector<Point> all_points;
vector<ld> max_angle_differences;
vector<int> top_half_indices;

bool compare_angle(const Point &a, const Point &b) {
    ld angle_a = corrected_angle(one_zero, origin_point, a);
    ld angle_b = corrected_angle(one_zero, origin_point, b);
    return angle_a < angle_b;
}

int main() {
    int test_cases;
    cin >> test_cases;
    while (test_cases--) {
        cin >> num_points >> distance_limit;
        all_points.clear();
        top_half_indices.clear();

        for (int i = 0; i < num_points; ++i) {
            Point p;
            cin >> p.x >> p.y;
            if (distance(origin_point, p) <= distance_limit) {
                continue;  // ignore points within distance_limit from origin
            }
            all_points.push_back(p);
        }

        num_points = all_points.size();
        sort(all_points.begin(), all_points.end(), compare_angle);

        max_angle_differences.resize(num_points);
        for (int i = 0; i < num_points; ++i) {
            ld left_angle = corrected_angle(one_zero, origin_point, all_points[i]);
            ld right_angle = left_angle + M_PI / 2.0;  // 90 degrees

            for (int j = 0; j < 50; ++j) {
                ld middle_angle = (left_angle + right_angle) / 2.0;
                Point p(cos(middle_angle), sin(middle_angle));
                Line line_to_point(origin_point, p);

                if (distToLine(all_points[i], line_to_point, p) <= distance_limit) {
                    left_angle = right_angle;
                                        left_angle = right_angle;  // can have a bigger angle
                } else {
                    right_angle = middle_angle;
                }
            }
            max_angle_differences[i] = left_angle - corrected_angle(one_zero, origin_point, all_points[i]);
        }

        ld min_lines = num_points;
        for (int i = 0; i < num_points; ++i) {
            int current_point = i;
            ld current_result = 1;
            for (int j = 1; j < num_points; ++j) {
                int next_point = (i + j) % num_points;
                if (max_angle_differences[next_point] + max_angle_differences[current_point] >=
                    angle(all_points[current_point], origin_point, all_points[next_point])) {
                    continue;
                } else {
                    current_result++;
                    current_point = next_point;
                }
            }
            min_lines = min(min_lines, current_result);
        }

        cout << min_lines << endl;
    }

    return 0;
}

