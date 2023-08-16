#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>

using namespace std;

struct Customer {
    int id;
    int processing_time;
    int start_time;
    int end_time;
};

struct CompareCustomer {
    bool operator()(Customer const& c1, Customer const& c2) {
        return c1.processing_time > c2.processing_time;
    }
};

int main() {
    srand(time(NULL));

    int N; 
    cout << "Müşteri sayısını giriniz: "; 
    cin>>N;
    int min_time = 30;
    int max_time = 300;

    priority_queue<Customer, vector<Customer>, CompareCustomer> pq;
    stack<Customer> s;

//stack yapısı olusturuldu

    for (int i = 1; i <= N; i++) {
        int processing_time = rand() % (max_time - min_time + 1) + min_time;
        Customer c = { i, processing_time, 0, 0 };
        s.push(c);
    }

    int current_time = 0;
    int total_time = 0;
    int count = 0;

    while (!pq.empty() || !s.empty()) {
        
        while (!s.empty() && s.top().processing_time + current_time <= 1440) {
            Customer c = s.top();
            s.pop();
            c.start_time = current_time;
            c.end_time = current_time + c.processing_time;
            pq.push(c);
        }

        
        if (!pq.empty()) {
            Customer c = pq.top();
            pq.pop();
            total_time += c.end_time - c.start_time;
            count++;
            current_time = c.end_time;
            cout << "Musteri: " << c.id << " Süre: " << c.processing_time
                 << " saniye. Toplam süre: " << total_time << endl;
        } else {
            Customer c = s.top();
            current_time = c.start_time = c.end_time = current_time + c.processing_time;
            s.pop();
            pq.push(c);
        }
    }

    cout << "Ortalama süre: " << (float)total_time / count << endl;

    while (!s.empty()) {
        Customer c = s.top();
        s.pop();
        if (c.start_time - current_time > 0) {
            cout << "Müşteri " << c.id << " öncelik kuyruğunda FIFO kuyruğundan daha uzun süre bekledi." << endl;
        }
    }

    return 0;
}