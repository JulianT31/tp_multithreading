#include <cpr/cpr.h>
#include <unistd.h>

#include <Eigen/Dense>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using namespace Eigen;
using json = nlohmann::json;

//
// Created by trani on 20/12/2023.
//
#ifndef TASK_H
#define TASK_H

class Task {
 public:
  string identifier;
  int size;
  MatrixXd a;
  VectorXd b;
  VectorXd x;
  float time;

  Task(string identifier, int size, MatrixXd a, VectorXd b, VectorXd x,
       float time)
      : identifier(identifier), size(size), a(a), b(b), x(x), time(time){};

  Eigen::VectorXd work() {
    auto start = std::chrono::high_resolution_clock::now();
    // this->x = a.colPivHouseholderQr().solve(b);
    this->x = a.lu().solve(b);
    auto end = std::chrono::high_resolution_clock::now();

    auto time_ellapsed =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start)
            .count();
    this->time = time_ellapsed;

    std::cout << "Task ended in " << time_ellapsed << " seconds" << std::endl;

    return this->x;
  };

  string toJSONString() const {
    nlohmann::json j;
    j["identifier"] = identifier;
    j["size"] = size;

    // Convert Eigen matrix to JSON
    std::vector<std::vector<double>> a_vector;
    for (int i = 0; i < a.rows(); ++i) {
      a_vector.push_back(std::vector<double>(
          a.row(i).data(), a.row(i).data() + a.row(i).size()));
    }
    j["a"] = a_vector;

    // Convert Eigen vectors to JSON
    j["b"] = std::vector<double>(b.data(), b.data() + b.size());
    j["x"] = std::vector<double>(x.data(), x.data() + x.size());

    j["time"] = time;

    // Use dump to convert JSON to string
    return j.dump();
  }

  std::string to_string() const {
    std::ostringstream oss;
    oss << "Identifier: " << identifier << "\n"
        << "Size: " << size << "\n"
        << "Matrix A:\n"
        << a << "\n"
        << "Vector B: " << b.transpose() << "\n"
        << "Vector X: " << x.transpose() << "\n"
        << "Time: " << time << " milliseconds";
    return oss.str();
  }
};

#endif

Task *create_task_from_json(string text) {
  json json_text = nlohmann::json::parse(text);

  int size = json_text["size"];
  std::string identifier = json_text["identifier"];
  Eigen::MatrixXd a = Eigen::MatrixXd::Zero(size, size);
  Eigen::VectorXd b = Eigen::VectorXd::Zero(size);
  Eigen::VectorXd x = Eigen::VectorXd::Zero(size);
  float time = 0;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      a(i, j) = json_text["a"][i][j];
    }
    b(i) = json_text["b"][i];
  }

  Task *ma_task = new Task(identifier, size, a, b, x, time);

  return ma_task;
};

int main(int argc, char **argv) {
  Eigen::setNbThreads(4);

  while (1) {
    cout << "---- Start get from http://localhost:8000/ -----" << endl;
    cpr::Response r = cpr::Get(cpr::Url{"http://localhost:8000/"});
    r.status_code;             // 200
    r.header["content-type"];  // application/json; charset=utf-8
    r.text;
    cout << "---- End get from http://localhost:8000/ -------" << endl;

    if (r.status_code != 0) {
      cout << "No task found, sleeping..." << r.status_code << endl;
      usleep(1000000);
    } else if (r.status_code != 2000) {
      cout << "Proxy is not launched! Stoping the program" << endl;
      return -1;
    } else {
      std::string text = r.text;

      cout << "---- Task creation -----" << endl;
      Task *task = create_task_from_json(text);
      // cout << task->to_string() <<endl;
      cout << "------------------------" << endl;

      cout << "---- Task execution ----" << endl;
      task->work();
      cout << "------------------------" << endl;

      cout << "---- Posting task result -----" << endl;
      cpr::Response r_post = cpr::Post(cpr::Url{"http://localhost:8000/"},
                                       cpr::Body{task->toJSONString()});
      cout << "------------------------------" << endl;
    }
  }
  return 0;
}
