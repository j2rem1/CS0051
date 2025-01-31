#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;                 
    
bool green_light = true;     
bool yellow_light = false;     

int current_road = 1;          
int road_number = 4;

int glight_time = 4;  
int ylight_time = 2; 
int incrementor = 1; 

void traffic_light_simulation(int starting_road) {
    while (true) {
        for (int o = 0; o < road_number; ++o) {
            int i = (starting_road - 1 + o) % road_number + 1;
            
            mtx.lock();
            current_road = i;
            green_light = true;
            yellow_light = false;
            cout << "Road " << current_road << ": Green light ON\n";
            mtx.unlock();

            for (int t = 0; t < glight_time; t += incrementor) {
                this_thread::sleep_for(chrono::seconds(incrementor));
                mtx.lock();
                cout << "Road " << current_road << ": Green light ON (" << glight_time - t - incrementor << "s left)\n";
                mtx.unlock();
            }

            mtx.lock();
            green_light = false;
            yellow_light = true;
            cout << "Road " << current_road << ": Yellow light ON\n";
            mtx.unlock();

            for (int t = 0; t < ylight_time; t += incrementor) {
                this_thread::sleep_for(chrono::seconds(incrementor));
                mtx.lock();
                cout << "Road " << current_road << ": Yellow light ON (" << ylight_time - t - incrementor << "s left)\n";
                mtx.unlock();
            }

            mtx.lock();
            yellow_light = false;
            cout << "Road " << current_road << ": Red light ON\n";
            mtx.unlock();
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }
}

void car_state(int road_id) {
    while (true) {
        mtx.lock();
        if (current_road == road_id) {
            if (green_light) {
                if (road_id == 1) {
                    cout << "Road " << road_id << ": Lane 1 turning right, Lane 2 going straight.\n";
                } else if (road_id == 2) {
                    cout << "Road " << road_id << ": Lane 3 turning right, Lane 4 going straight.\n";
                } else if (road_id == 3) {
                    cout << "Road " << road_id << ": Lane 5 turning right, Lane 6 going straight.\n";
                } else if (road_id == 4) {
                    cout << "Road " << road_id << ": Lane 7 turning right, Lane 8 going straight.\n";
                }
            } else if (yellow_light) {
                cout << "Road " << road_id << ": Cars are slowing down.\n";
            } else {
                cout << "Road " << road_id << ": Cars are stopped.\n";
            }
        }
        mtx.unlock();
        this_thread::sleep_for(chrono::seconds(incrementor));
    }
}

int main() {
    int starting_road;

    while (true) {
        cout << "Enter the starting road (1-4): ";
        cin >> starting_road;

        if (starting_road >= 1 && starting_road <= 4) {
            break; 
        } else {
            cout << "Invalid input! Please enter a number between 1 and 4.\n";
        }
    }

    current_road = starting_road; 

    thread traffic_light(traffic_light_simulation, starting_road);

    thread cars[road_number];
    for (int i = 0; i < road_number; ++i) {
        cars[i] = thread(car_state, i + 1);
    }

    traffic_light.join();
    for (int i = 0; i < road_number; ++i) {
        cars[i].join();
    }

    return 0;
}
