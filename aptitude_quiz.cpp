// ============================================================
//  APTITUDE QUIZ - C++ Mini Project
//  A randomised aptitude test with instant feedback,
//  topic-wise stats and a final performance report.
//
//  Compile : g++ -std=c++17 -o aptitude_quiz aptitude_quiz.cpp
//  Run     : ./aptitude_quiz
// ============================================================

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>

using namespace std;

// ---------- random engine ----------
mt19937 rng(random_device{}());

int randInt(int lo, int hi) {
    return uniform_int_distribution<int>(lo, hi)(rng);
}

// ---------- question model ----------
struct Question {
    string topic;
    string text;
    vector<string> options;   // 4 choices
    int correct = 0;          // index of the correct option
    string solution;          // explanation shown after answering
};

// ---------- helpers ----------
int readInt(const string& prompt, int lo, int hi) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= lo && value <= hi) return value;
        cout << "  Please enter a number between " << lo << " and " << hi << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Builds a question where the options are auto-generated
// around the correct numeric answer (idea: random question generator).
Question makeQuestion(const string& topic, const string& text, int answer,
                      const string& solution) {
    vector<int> values = {answer};
    int spread = max(3, abs(answer) / 6);
    while ((int)values.size() < 4) {
        int w = answer + randInt(-spread, spread);
        if (w < 0 || find(values.begin(), values.end(), w) != values.end()) continue;
        values.push_back(w);
    }
    shuffle(values.begin(), values.end(), rng);

    Question q;
    q.topic = topic;
    q.text = text;
    q.solution = solution;
    for (int v : values) q.options.push_back(to_string(v));
    q.correct = (int)(find(values.begin(), values.end(), answer) - values.begin());
    return q;
}

// ---------- fixed question bank ----------
vector<Question> fixedBank() {
    return {
        // ----- Percentages -----
        {"Percentages",
         "What is 20% of 150?",
         {"25", "30", "35", "40"}, 1,
         "20/100 x 150 = 30"},

        {"Percentages",
         "A number increased by 20% becomes 240. The original number is:",
         {"180", "200", "192", "216"}, 1,
         "Original = 240 x 100/120 = 200"},

        {"Percentages",
         "A single discount equivalent to successive discounts of 20% and 10% is:",
         {"27%", "28%", "30%", "32%"}, 1,
         "1 - (0.80 x 0.90) = 1 - 0.72 = 0.28 -> 28%"},

        // ----- Profit & Loss -----
        {"Profit & Loss",
         "An item costing Rs 500 is sold at 15% profit. The selling price is:",
         {"Rs 550", "Rs 565", "Rs 575", "Rs 585"}, 2,
         "SP = 500 + 15% of 500 = 500 + 75 = Rs 575"},

        {"Profit & Loss",
         "An article sold for Rs 720 at a 20% loss. Its cost price was:",
         {"Rs 840", "Rs 864", "Rs 900", "Rs 960"}, 2,
         "CP = 720 x 100/80 = Rs 900"},

        {"Profit & Loss",
         "CP is Rs 480 and SP is Rs 540. The profit percentage is:",
         {"10%", "12%", "12.5%", "15%"}, 2,
         "Profit = 60; 60/480 x 100 = 12.5%"},

        // ----- Time, Speed & Distance -----
        {"Time Speed Distance",
         "Average speed for a 150 km journey covered in 2.5 hours:",
         {"55 km/h", "60 km/h", "65 km/h", "70 km/h"}, 1,
         "Speed = 150 / 2.5 = 60 km/h"},

        {"Time Speed Distance",
         "A 150 m long train at 54 km/h crosses a pole in:",
         {"8 s", "10 s", "12 s", "15 s"}, 1,
         "54 km/h = 15 m/s; time = 150 / 15 = 10 s"},

        {"Time Speed Distance",
         "Two trains 300 km apart move toward each other at 60 and 40 km/h."
         " They meet after:",
         {"2.5 h", "3 h", "3.5 h", "4 h"}, 1,
         "Relative speed = 100 km/h; 300 / 100 = 3 h"},

        // ----- Ratio & Proportion -----
        {"Ratio",
         "If a : b = 3 : 4 and b : c = 2 : 5, then a : c =",
         {"3 : 10", "4 : 15", "5 : 12", "6 : 25"}, 0,
         "Make b equal (LCM 4): b : c = 4 : 10, so a : c = 3 : 10"},

        {"Ratio",
         "Rs 240 is divided in the ratio 5 : 3. The larger share is:",
         {"Rs 90", "Rs 100", "Rs 140", "Rs 150"}, 3,
         "Larger share = 5/8 x 240 = Rs 150"},

        // ----- Average -----
        {"Average",
         "The average of 10, 15, 20, 25 and 30 is:",
         {"18", "20", "22", "25"}, 1,
         "Sum = 100; 100 / 5 = 20"},

        {"Average",
         "The average of 5 numbers is 24. If 36 is included, the new average is:",
         {"26", "27", "28", "25"}, 0,
         "(5 x 24 + 36) / 6 = 156 / 6 = 26"},

        // ----- Time & Work -----
        {"Time & Work",
         "A can do a job in 10 days and B in 15 days. Working together they need:",
         {"5 days", "6 days", "7.5 days", "8 days"}, 1,
         "1/10 + 1/15 = 3/30 + 2/30 = 1/6 -> 6 days"},

        {"Time & Work",
         "A is twice as fast as B. Together they finish in 12 days."
         " A alone takes:",
         {"16 days", "18 days", "20 days", "24 days"}, 1,
         "Rates: 2k + k = 1/12 -> k = 1/36; A = 2k = 1/18 -> 18 days"},
    };
}

// ---------- auto-generated questions ----------
Question genPercentage() {
    int k = randInt(10, 40);   // number = 20k
    int m = randInt(2, 8);     // percent = 5m (10% - 40%)
    int n = 20 * k, p = 5 * m;
    int ans = n * p / 100;     // = k * m (always whole)
    return makeQuestion(
        "Percentages",
        "What is " + to_string(p) + "% of " + to_string(n) + "?",
        ans,
        to_string(p) + "/100 x " + to_string(n) + " = " + to_string(ans));
}

Question genProfitLoss() {
    int cp = 20 * randInt(5, 29);   // 100 - 580 (keeps maths whole)
    int p = 5 * randInt(1, 8);      // 5% - 40%
    int sp = cp + cp * p / 100;
    return makeQuestion(
        "Profit & Loss",
        "An item bought for Rs " + to_string(cp) + " is sold at " +
            to_string(p) + "% profit. The selling price is:",
        sp,
        "SP = CP + CP x P/100 = " + to_string(cp) + " + " +
            to_string(cp * p / 100) + " = Rs " + to_string(sp));
}

Question genTSD() {
    int speed = 10 * randInt(3, 8);   // 30 - 80 km/h
    int t2 = randInt(2, 7);           // time = t2 / 2 hours
    int dist = speed * t2 / 2;
    string timeStr = (t2 % 2 == 0) ? to_string(t2 / 2)
                                   : to_string(t2 / 2) + ".5";
    return makeQuestion(
        "Time Speed Distance",
        "A car travels at " + to_string(speed) + " km/h for " + timeStr +
            " hours. The distance covered is:",
        dist,
        "Distance = Speed x Time = " + to_string(speed) + " x " + timeStr +
            " = " + to_string(dist) + " km");
}

// ---------- quiz engine ----------
void runQuiz(const vector<Question>& quiz) {
    map<string, array<int, 2>> stats;   // topic -> {correct, total}
    int score = 0;

    cout << "\n========================================\n"
         << "               QUIZ START\n"
         << "========================================\n";

    for (size_t i = 0; i < quiz.size(); ++i) {
        const Question& q = quiz[i];
        stats[q.topic][1]++;

        cout << "\nQ" << i + 1 << "  [" << q.topic << "]\n"
             << q.text << "\n";
        for (size_t j = 0; j < q.options.size(); ++j)
            cout << "  " << j + 1 << ") " << q.options[j] << "\n";

        int ans = readInt("Your answer (1-4): ", 1, 4) - 1;
        if (ans == q.correct) {
            score++;
            stats[q.topic][0]++;
            cout << "  + Correct!\n";
        } else {
            cout << "  - Wrong. Correct answer: "
                 << q.options[q.correct] << "\n";
        }
        cout << "  Why: " << q.solution << "\n";
    }

    // ---------- final report ----------
    int total = (int)quiz.size();
    double pct = total ? 100.0 * score / total : 0.0;
    string grade;
    if (pct >= 90) grade = "Excellent";
    else if (pct >= 75) grade = "Very Good";
    else if (pct >= 50) grade = "Good";
    else grade = "Keep Practising";

    cout << "\n========================================\n"
         << "             FINAL REPORT\n"
         << "========================================\n"
         << " Score      : " << score << " / " << total
         << "  (" << pct << "%)\n"
         << " Grade      : " << grade << "\n"
         << " Topic-wise accuracy:\n";

    for (const auto& entry : stats) {
        const string& topic = entry.first;
        int c = entry.second[0], t = entry.second[1];
        double tp = t ? 100.0 * c / t : 0.0;
        cout << "   " << topic;
        cout << string(max(1, 22 - (int)topic.size()), ' ');
        cout << ": " << c << "/" << t << "  (" << tp << "%)\n";
    }
    cout << "========================================\n";
}

// ---------- main ----------
int main() {
    cout << "========================================\n"
         << "        APTITUDE QUIZ (C++)\n"
         << "  Percentages | Profit & Loss | Speed\n"
         << "  Ratio | Average | Time & Work\n"
         << "========================================\n";

    while (true) {
        // Build the pool: fixed bank + fresh random questions
        vector<Question> pool = fixedBank();
        for (int i = 0; i < 5; ++i) {
            pool.push_back(genPercentage());
            pool.push_back(genProfitLoss());
            pool.push_back(genTSD());
        }

        // Collect topic list
        vector<string> topics = {"All topics"};
        for (const Question& q : pool)
            if (find(topics.begin(), topics.end(), q.topic) == topics.end())
                topics.push_back(q.topic);

        cout << "\nChoose a topic:\n";
        for (size_t i = 0; i < topics.size(); ++i)
            cout << "  " << i << ") " << topics[i] << "\n";

        int choice = readInt("> ", 0, (int)topics.size() - 1);

        vector<Question> filtered;
        if (choice == 0) filtered = pool;
        else
            copy_if(pool.begin(), pool.end(), back_inserter(filtered),
                    [&](const Question& q) { return q.topic == topics[choice]; });

        int maxQ = (int)filtered.size();
        int n = readInt("How many questions? (1-" + to_string(maxQ) + "): ",
                        1, maxQ);

        shuffle(filtered.begin(), filtered.end(), rng);
        filtered.resize(n);

        runQuiz(filtered);

        char again;
        cout << "\nPlay again? (y/n): ";
        cin >> again;
        if (tolower(again) != 'y') break;
    }

    cout << "\nThanks for playing. All the best!\n";
    return 0;
}
