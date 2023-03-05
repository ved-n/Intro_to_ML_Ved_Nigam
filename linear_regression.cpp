#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

// Define the sigmoid function
double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

// Define the logistic regression function
void logisticRegression(vector<vector<double>> train_data, vector<double> train_labels, double &w0, double &w1, int epochs, double learning_rate)
{
    int n = train_data.size();
    double error = 0;
    double prev_error = 0;
    w0 = 0;
    w1 = 0;

    // Perform gradient descent
    for (int epoch = 0; epoch < epochs; epoch++)
    {
        error = 0;
        for (int i = 0; i < n; i++)
        {
            double prediction = sigmoid(w0 + w1 * train_data[i][0]);
            double label = train_labels[i];
            double delta = learning_rate * (label - prediction);
            w0 += delta;
            w1 += delta * train_data[i][0];
            error += pow(label - prediction, 2);
        }
        // Stop if the error hasn't changed significantly
        if (abs(error - prev_error) < 0.0001)
        {
            break;
        }
        prev_error = error;
    }
}

// Define the predict function
vector<double> predict(vector<vector<double>> test_data, double w0, double w1)
{
    int n = test_data.size();
    vector<double> predictions(n);
    for (int i = 0; i < n; i++)
    {
        double prediction = sigmoid(w0 + w1 * test_data[i][0]);
        predictions[i] = round(prediction);
    }
    return predictions;
}

// Define the accuracy function
double accuracy(vector<double> predictions, vector<double> labels)
{
    int n = predictions.size();
    int correct = 0;
    for (int i = 0; i < n; i++)
    {
        if (predictions[i] == labels[i])
        {
            correct++;
        }
    }
    return static_cast<double>(correct) / n;
}

// Define the sensitivity function
double sensitivity(vector<double> predictions, vector<double> labels)
{
    int n = predictions.size();
    int true_positives = 0;
    int actual_positives = 0;
    for (int i = 0; i < n; i++)
    {
        if (labels[i] == 1)
        {
            actual_positives++;
            if (predictions[i] == 1)
            {
                true_positives++;
            }
        }
    }
    if (actual_positives == 0)
    {
        return 0;
    }
    else
    {
        return static_cast<double>(true_positives) / actual_positives;
    }
}

// Define the specificity function
double specificity(vector<double> predictions, vector<double> labels)
{
    int n = predictions.size();
    int true_negatives = 0;
    int actual_negatives = 0;
    for (int i = 0; i < n; i++)
    {
        if (labels[i] == 0)
        {
            actual_negatives++;
                if (predictions[i] == 0)
            {
                true_negatives++;
            }
        }
    }
    if (actual_negatives == 0)
    {
        return 0;
    }
    else
    {
        return static_cast<double>(true_negatives) / actual_negatives;
    }
}

int main() {
    // Read in the Titanic data
    vector<vector<double>> data;
    vector<double> labels;
    ifstream file("titanic_project.csv");
    string line;
    getline(file, line); // Skip the header line

    // Read in the data
    while (getline(file, line)){
        stringstream ss(line);
        string value;
        vector<double> row;
        for (int i = 0; getline(ss, value, ','); i++)
        {
            if (i == 2)
            {
                labels.push_back(stod(value));
            }
            else if (i == 3)
            {
                if (value == "male")
                {
                    row.push_back(0);
                }
                else
                {
                    row.push_back(1);
                }
            }
        }
        data.push_back(row);
    }

    // Split the data into training and test sets
    int n_train = 800;
    vector<vector<double>> train_data(data.begin(), data.begin() + n_train);
    vector<double> train_labels(labels.begin(), labels.begin() + n_train);
    vector<vector<double>> test_data(data.begin() + n_train, data.end());
    vector<double> test_labels(labels.begin() + n_train, labels.end());

    // Perform logistic regression on the training data
    double w0, w1;
    int epochs = 1000;
    double learning_rate = 0.001;
    auto start_time = chrono::high_resolution_clock::now();
    logisticRegression(train_data, train_labels, w0, w1, epochs, learning_rate);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << "Training time: " << duration.count() << " microseconds" << endl;

    // Output the coefficients
    cout << "w0: " << w0 << endl;
    cout << "w1: " << w1 << endl;

    // Use the model to predict the values for the test data
    vector<double> predictions = predict(test_data, w0, w1);

    // Calculate the accuracy, sensitivity, and specificity
    double acc = accuracy(predictions, test_labels);
    double sens = sensitivity(predictions, test_labels);
    double spec = specificity(predictions, test_labels);

    // Output the test metrics
    cout << "Accuracy: " << acc << endl;
    cout << "Sensitivity: " << sens << endl;
    cout << "Specificity: " << spec << endl;

    return 0;
}