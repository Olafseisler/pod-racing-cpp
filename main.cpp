#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

#define PI 3.14159265358979323846

using namespace std;

class Point {
protected:
    int y_coord = 0;
    int x_coord = 0;
public:
    Point() = default;

    Point(int x, int y) {
        this->x_coord = x;
        this->y_coord = y;
    }

    [[nodiscard]] Point calculateMidpoint(const Point &other) const {
        float midpointX = (x_coord + other.x_coord) / 2.0;
        float midpointY = (y_coord + other.y_coord) / 2.0;
        return Point(midpointX, midpointY);
    }

    [[nodiscard]] int get_x() const { return x_coord; }

    [[nodiscard]] int get_y() const { return y_coord; }

    void set_coords(int x, int y) {
        this->x_coord = x;
        this->y_coord = y;
    }

    float distance_to(Point p) {
        return sqrt(pow(x_coord - p.x_coord, 2) + pow(y_coord - p.y_coord, 2));
    }
};

class RacingPod : public Point {
protected:
    float angle;
    int next_checkpoint;
    float vx;
    float vy;

public:
    RacingPod() : Point() {}

    float get_angle(Point p) {
        float dist = this->distance_to(p);
        float dx = (p.get_x() - this->x_coord) / dist;
        float dy = (p.get_y() - this->y_coord) / dist;
        float a = acos(dx) * 180 / PI;

        if (dy < 0) {
            a = 360.0f - a;
        }

        return a;
    }

    float get_angle_to(Point p) {
        float target_angle = this->get_angle(p);
        float right = this->angle <= target_angle ? target_angle - this->angle : 360.0 - this->angle + target_angle;
        float left = this->angle >= target_angle ? this->angle - target_angle : this->angle + 360.0 - target_angle;
        return right < left ? right : -left;
    }

    void rotate(Point p) {
        float a = this->get_angle_to(p);

        // Can't turn by more than 18° in one turn
        if (a > 18.0) {
            a = 18.0;
        } else if (a < -18.0) {
            a = -18.0;
        }

        this->angle += a;

        // The % operator is slow. If we can avoid it, it's better.
        if (this->angle >= 360.0) {
            this->angle = this->angle - 360.0;
        } else if (this->angle < 0.0) {
            this->angle += 360.0;
        }
    }

    void boost(int thrust) {
        // Conversion of the angle to radians
        float ra = this->angle * PI / 180.0;

        // Trigonometry
        this->vx += cos(ra) * thrust;
        this->vy += sin(ra) * thrust;
    }

    void move() {
        this->x_coord += this->vx;
        this->y_coord += this->vy;
    }

    void end() {
        this->x_coord = round(this->x_coord);
        this->y_coord = round(this->y_coord);
        this->vx = (int) (this->vx * 0.85);
        this->vy = (int) (this->vy * 0.85);
    }

    void play(Point p, int thrust) {
        this->rotate(p);
        this->boost(thrust);
        this->move();
        this->end();
    }
};

int next_checkpoint_dist_cached = 0;
int next_checkpoint_x_cached = 0;
int next_checkpoint_y_cached = 0;
vector<Point> checkpoints;
int checkpoint_index = 0;
bool passed_last = false;
bool visited_all = false;

bool checkpoint_exists(int next_x, int next_y) {
    for (size_t i = 0; i < checkpoints.size(); ++i) {
        if (checkpoints[i].get_x() == next_x &&
            checkpoints[i].get_y() == next_y) {
            if (checkpoints.size() > 1 && i == 0){
                visited_all = true;
                cerr << "size: " << checkpoints.size() << endl;
            }

            return true;
        }
    }
    return false;
}

void set_next_checkpoint(RacingPod &player, Point &current_checkpoint, int next_dist, int next_x, int next_y) {
    cerr << "set next called!" << endl;

    if (!checkpoint_exists(next_x, next_y))
    { // New point from stream
        cerr << "in new checkpoint branch, index " << checkpoint_index << endl;
        cerr << "next x, y: " << next_x << ", " << next_y << endl;
        Point cp(next_x, next_y);
        checkpoints.push_back(cp);
        current_checkpoint = checkpoints[checkpoint_index];
        next_checkpoint_dist_cached =  next_dist;
        ++checkpoint_index;
        passed_last = true;
    } else {
        if (current_checkpoint.get_x() != next_x &&
            current_checkpoint.get_y() != next_y)
        { // point from stream is not the current point
            cerr << "next point is not current" << endl;
            if (passed_last){
                current_checkpoint = checkpoints[checkpoint_index % checkpoints.size()];
                next_checkpoint_dist_cached = next_dist;
                ++checkpoint_index;
//                passed_last = false;
            }
        } else { // point from stream is current
            passed_last = true;
            if ((float)next_dist / next_checkpoint_dist_cached < 0.2) {
                cerr << "less than 20%" << endl;
                if (visited_all){
                    cerr << "set new point preemptively" << endl;
                    current_checkpoint = checkpoints[checkpoint_index % checkpoints.size()];
                    next_checkpoint_dist_cached = player.distance_to(current_checkpoint);
                    ++checkpoint_index;
                    passed_last = false;
                }
            }
        }
    }

    // cerr << "cached coords: " << next_checkpoint_x_cached << ", "
    //      << next_checkpoint_y_cached << endl;

    for (int j = 0; j < checkpoints.size(); ++j)
        cerr << "checkpoint " << j << " x: " << checkpoints[j].get_x() << " y: " << checkpoints[j].get_x() << endl;
}

int main() {

    Point current_checkpoint;
    RacingPod player;

    // game loop
    while (1) {
        int x;
        int y;
        int next_checkpoint_x; // x position of the next check point
        int next_checkpoint_y; // y position of the next check point
        int next_checkpoint_dist; // distance to the next checkpoint
        int next_checkpoint_angle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> next_checkpoint_x >> next_checkpoint_y >> next_checkpoint_dist >> next_checkpoint_angle;
        cin.ignore();
        int opponent_x;
        int opponent_y;
        cin >> opponent_x >> opponent_y;
        cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // Detect when a new checkpoint is added
        player.set_coords(x, y);
        set_next_checkpoint(player, current_checkpoint, next_checkpoint_dist, next_checkpoint_x, next_checkpoint_y);

        float dist_factor = 1 / (1 + exp(-20 * ((float) next_checkpoint_dist / next_checkpoint_dist_cached - 0.25)));
        int thrust = dist_factor * 100 * exp(-0.00005 * pow(next_checkpoint_angle, 2));

//        player.play(current_checkpoint, thrust);

        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        cout << current_checkpoint.get_x() << " " << current_checkpoint.get_y() << " " << to_string(thrust) << endl;
    }

}