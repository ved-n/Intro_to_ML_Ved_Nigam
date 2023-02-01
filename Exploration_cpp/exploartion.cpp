#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

double sumFunc(vector<double> data) {
    int size = data.size();
    double sum = 0;

    for (int i = 0; i < size; i++) {
        sum += data[i];
    }

    return sum;
}

double meanFunc(vector<double> data) {
    int size = data.size();
    double sum = 0;
    double mean;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    mean = sum / size;

    return mean;
}

double medianFunc(vector<double> data) {
    int size = data.size();
    double median;
    sort(data.begin(), data.end());

    if (size % 2 == 1) {
        median = data[(size / 2) + 1];
    } else {
        median = (data[(size / 2)] + data[(size / 2) + 1]) / 2.0;
    }

    return median;
}

double rangeFunc(vector<double> data) {
    int size = data.size();
    sort(data.begin(), data.end());

    double min = data[0];
    double max = data[size - 1];
    double range = max - min;

    return range;
}

void print_stats(vector<double> data) {
    int size = data.size();
    
    double sum = sumFunc(data);
    cout << "Sum is " << sum << endl;

    double mean = meanFunc(data);
    cout << "Mean is " << mean << endl;

    double median = medianFunc(data);
    cout << "Median is " << median << endl;
    
    double range = rangeFunc(data);
    cout << "Range is " << range << endl;
}

double covar(vector<double> data1, vector<double> data2) {
    int size = data1.size(); // Both vectors have same size since from same data set
    double meanOne = meanFunc(data1);
    double meanTwo = meanFunc(data2);

    double runningSum = 0.0;
    for (int i = 0; i < size; i++) {
        runningSum += ((data1[i] - meanOne) * (data2[i] - meanTwo));
    }

    double covariance = runningSum / (size - 1);

    return covariance;
}

double var(vector<double> data) {
    int size = data.size();
    double dataMean = meanFunc(data);
    double runningSum = 0;

    for (int i = 0; i < size; i++) {
        runningSum += ((data[i] - dataMean) * (data[i] - dataMean));
    }

    double variance = runningSum / (size - 1);

    return variance;
}

double cor(vector<double> data1, vector<double> data2) {
    int size = data1.size(); // Both vectors have the same size since from the same data set
    double covariance = covar(data1, data2);
    double sigmaOne = sqrt(var(data1));
    double sigmaTwo = sqrt(var(data2));

    double correlation = covariance / (sigmaOne * sigmaTwo);

    return correlation;
}

int main(int argc, char** argv) {
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    cout << "Opening Boston.csv" << endl;

    inFS.open("Boston.csv");
    if (!inFS.is_open()) {
        cout << "Could not open Boston.csv" << endl;
        return 1; //error
    }

    cout << "Reading in line 1" << endl;
    getline(inFS, line);

    cout << "heading " << line << endl;

    int numOfObservations = 0;
    while (inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numOfObservations) = stof(rm_in);
        medv.at(numOfObservations) = stof(medv_in);

        numOfObservations++;
    }

    rm.resize(numOfObservations);
    medv.resize(numOfObservations);

    cout << "New length is " << rm.size() << endl;

    cout << "Closing file" << endl;
    inFS.close();

    cout << "Number of records is " << numOfObservations << endl;

    cout << "\n Stats for rm" << endl;
    print_stats(rm);

    cout << "\n Stats for medv" << endl;
    print_stats(medv);

    cout << "\n Covariance is " << covar(rm, medv) << endl;

    cout << "\n Correlation is " << cor(rm, medv) << endl;

    cout << "\n Program ended" << endl;

    return 0;
}