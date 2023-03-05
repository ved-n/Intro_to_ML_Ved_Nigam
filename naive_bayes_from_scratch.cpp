#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;

// perform Naive Bayes
void naiveBayes(vector<int> index, vector<int> p_class, vector<int> survived, vector<int> sex, vector<int> age) {
    // Counting survival data
    int survived_count = 0;
    int dead_count = 0;
    double total = survived.size();

    for (int i = 0; i < survived.size(); i++) {
        if(survived[i] == 1) {
            survived_count++;
        } else {
            dead_count++;
        }
    }

    double survivedProb = survived_count / total;
    double deadProb = dead_count / total;

    // Making vector of survival probabilities
    vector<double> survivalProbs;
    survivalProbs.push_back(survivedProb);
    survivalProbs.push_back(deadProb);

    // Making vector of survival counts
    vector<int> survived_counts;
    survived_counts.push_back(survived_count);
    survived_counts.push_back(dead_count);

    // Passenger class likelihood
    double class_one = 0;
    double class_two = 0;
    double class_three = 0;

    // Making vector of the counts in each passenger class
    vector<double> class_counts;
    for (int i = 0; i < p_class.size(); i++) {
        if (p_class[i] == 1) {
            class_one++;
        } else if (p_class[i] == 2) {
            class_two++;
        } else {
            class_three++;
        }
    }
    class_counts.push_back(class_one);
    class_counts.push_back(class_two);
    class_counts.push_back(class_three);

    // Making 2D vector of likelihoods of survival based on passenger class
    vector<vector<double> > p_class_lh(2, vector<double>(3));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            double sv_and_pc = 0.0;
            for (int k = 0; k < survived.size(); k++) {
                if (survived[k] == i && p_class[k] == (j + 1)) {
                    sv_and_pc++;
                }
            }
            p_class_lh[i][j] = (sv_and_pc / survived_counts[i]);
        }
    }

    // passenger class likelihood matrix
    cout << "passenger class likelihood matrix" << endl;
    for (int j = 0; j < 3; j++) {
        cout << p_class_lh[0][j] << " ";
    }
    cout << endl;
    for (int j = 0; j < 3; j++) {
        cout << p_class_lh[1][j] << " ";
    }
    cout << endl;

    // Sex likelihood
    // Making matrix of sex counts
    double male_count = 0;
    double female_count = 0;
    vector<double> sex_counts;
    
    for (int i = 0; i < sex.size(); i++) {
        if (sex[i] == 1) {
            male_count++;
        } else {
            female_count++;
        }
    }

    sex_counts.push_back(male_count);
    sex_counts.push_back(female_count);

    vector<vector<double> > sex_lh(2, vector<double>(2));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            double sv_and_sex = 0.0;
            for (int k = 0; k < sex.size(); k++) {
                if (survived[k] == i && sex[k] == j) {
                    sv_and_sex++;
                }
            }
            sex_lh[i][j] = (sv_and_sex / survived_counts[j]);
        }
    }

    // sex likelihood matrix
    cout << "sex likelihood matrix" << endl;
    for (int i = 0; i < 2; i++) {
        cout << sex_lh[0][i] << " ";
    }
    cout << endl;
    for (int i = 0; i < 2; i++) {
        cout << sex_lh[1][i] << " ";
    }
    cout << endl;

    // Likelihood for age
    // calculating means
    vector<double> age_means(2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < age.size(); j++) {
            if (survived[j] == i) {
                age_means[i] += age[j];
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        age_means[i] = age_means[i] / age.size();
    }

    cout << "means: " << endl;
    for (int i = 0; i < 2; i++) {
        cout << age_means[i] << " ";
    }
    cout << endl;

    // Calculating variances
    vector<double> age_vars(2);
    for (int i = 0; i < 2; i++) {
        double survived_age_sum = 0;
        for (int j = 0; j < age.size(); j++) {
            if (survived[j] == i) {
                survived_age_sum += pow((age[j] - age_means[i]), 2);
            }
        }
        age_vars[i] = sqrt(survived_age_sum);
    }

    for (int i = 0; i < 2; i++) {
        age_vars[i] = age_vars[i] / age.size();
    }

    cout << "variances: " << endl;
    for (int i = 0; i < 2; i++) {
        cout << age_vars[i] << " ";
    }
    cout << endl;

    // making age likelihood vector
    vector<double> age_lh(2);
    for (int i = 0; i < 2; i++) {
        // age_lh[i] = (1 / sqrt(2 * 3.1415 * age_vars[i]) * pow(e, -(())))
    }
}

int main(int argc, char **argv) {
    ifstream inFS;
    string line;
    string index_in, p_class_in, survived_in, sex_in, age_in;
    const int MAX_LEN = 2000;
    vector<int> index(MAX_LEN);
    vector<int> p_class(MAX_LEN);
    vector<int> survived(MAX_LEN);
    vector<int> sex(MAX_LEN);
    vector<int> age(MAX_LEN);

    inFS.open("titanic_project.csv");
    if (!inFS.is_open()) {
        cout << "Could not open titanic_project.csv" << endl;
        return 1; //error
    }

    getline(inFS, line);

    int numOfObservations = 0;
    while (inFS.good()) {
        getline(inFS, index_in, ',');
        getline(inFS, p_class_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in, '\n');


        index_in.erase(remove(index_in.begin(), index_in.end(), '"'), index_in.end());
        p_class_in.erase(remove(p_class_in.begin(), p_class_in.end(), '"'), p_class_in.end());
        survived_in.erase(remove(survived_in.begin(), survived_in.end(), '"'), survived_in.end());
        sex_in.erase(remove(sex_in.begin(), sex_in.end(), '"'), sex_in.end());
        age_in.erase(remove(age_in.begin(), age_in.end(), '"'), age_in.end());

        index.at(numOfObservations) = stoi(index_in);
        p_class.at(numOfObservations) = stoi(p_class_in);
        survived.at(numOfObservations) = stoi(survived_in);
        age.at(numOfObservations) = stoi(age_in);
        sex.at(numOfObservations) = stoi(sex_in);

        numOfObservations++;
    }

    index.resize(numOfObservations);
    p_class.resize(numOfObservations);
    survived.resize(numOfObservations);
    sex.resize(numOfObservations);
    age.resize(numOfObservations);

    vector<int> train_index(800);
    vector<int> train_p_class(800);
    vector<int> train_survived(800);
    vector<int> train_sex(800);
    vector<int> train_age(800);

    for (int i = 0; i < 800; i++) {
        train_index[i] = index[i];
        train_p_class[i] = p_class[i];
        train_survived[i] = survived[i];
        train_sex[i] = sex[i];
        train_age[i] = age[i];
    }

    naiveBayes(train_index, train_p_class, train_survived, train_sex, train_age);
}