#include <cpr/cpr.h>

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
    this->x = a.colPivHouseholderQr().solve(b);
    auto end = std::chrono::high_resolution_clock::now();

    this->time =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << "Task ended in " << time << " milliseconds" << std::endl;

    return this->x;
  };

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

Task* create_task_from_json(string text) {
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

  Task* ma_task = new Task(identifier, size, a, b, x, time);

  return ma_task;
};

int main(int argc, char** argv) {
  cout << "----Start get from http://localhost:8000/ -----" << endl;
  cpr::Response r = cpr::Get(cpr::Url{"http://localhost:8000/"});
  r.status_code;             // 200
  r.header["content-type"];  // application/json; charset=utf-8
  r.text;
  cout << "----End get from http://localhost:8000/ -----" << endl;

  if (r.status_code != 200) {
    cout << "Erreur " << r.status_code << endl;
    return -1;
  }

  cout << "----Test affichage du text Json recolte -----" << endl;
  std::string text = r.text;
  /* cout << text <<endl; */
  cout << "------------------------------------" << endl;

  cout << "----Test creation de task -----" << endl;
  Task* task = create_task_from_json(text);
  /* cout << task->to_string() <<endl; */
  cout << "------------------------------------" << endl;

  cout << "----Test execution task -----" << endl;
  task->work();
  cout << "------------------------------------" << endl;

  return 0;
}
