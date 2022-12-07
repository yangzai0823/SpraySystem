#pragma once

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class jsonParser : public rapidjson::Document {
 public:
  jsonParser() : rapidjson::Document() {}
  ~jsonParser() {}

  void read(const std::string& fileName) {
    std::ifstream infile(fileName, std::ios::in);
    std::string str((std::istreambuf_iterator<char>(infile)),
                    (std::istreambuf_iterator<char>()));
    infile.close();
    this->Parse(str.c_str(), str.size());
  }
  void write(const std::string& fileName) {
    if (this->IsNull()) {
      return;
    }
    rapidjson::StringBuffer ss;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> w(ss);
    this->Accept(w);
    std::ofstream file(fileName, std::ios::out | std::ios::trunc);
    file << ss.GetString() << std::endl;
    file.flush();
    file.close();
  }

  bool keyExists(const std::vector<std::string>& keys) {
    if (keys.empty()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    return rapidjson::Pointer(token).Get(*this) != nullptr;
  }

  static std::string toToken(const std::vector<std::string>& keys) {
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    return std::move(token);
  }

  template <typename T>
  int getValue(const std::vector<std::string>& keys, T& value) {
    if (keys.empty()) {
      return -1;
    }
    if (!this->IsObject()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    auto valuePtr = rapidjson::Pointer(token).Get(*this);
    if (valuePtr == nullptr) {
      return -1;
    }
    try {
      value = valuePtr->Get<T>();
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return -1;
    }
    return 0;
  }

  template <typename T>
  int getArray(const std::vector<std::string>& keys, std::vector<T>& value) {
    if (keys.empty()) {
      return -1;
    }
    if (!this->IsObject()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    auto valuePtr = rapidjson::Pointer(token).Get(*this);
    if (valuePtr == nullptr || !valuePtr->IsArray()) {
      return -1;
    }
    std::vector<T> v_;
    try {
      for (auto&& i : valuePtr->GetArray()) {
        v_.push_back(i.Get<T>());
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return -1;
    }
    value = std::move(v_);
    return 0;
  }

  template <typename T>
  int setValue(const std::vector<std::string>& keys, const T& value) {
    if (keys.empty()) {
      return -1;
    }
    if (!this->IsObject()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    rapidjson::Pointer(token).Set(*this, value);
    return 0;
  }

  int setArray(
      const std::vector<std::string>& keys,
      const rapidjson::Value& arr = rapidjson::Value(rapidjson::kObjectType)) {
    if (keys.empty()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    auto& allocator = this->GetAllocator();
    rapidjson::Pointer(token).Set(*this, arr, allocator);
  }

  template <typename T>
  int setArray(const std::vector<std::string>& keys,
               const std::vector<T>& values) {
    if (keys.empty()) {
      return -1;
    }
    if (!this->IsObject()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    rapidjson::Value arr(rapidjson::kArrayType);
    auto& allocator = this->GetAllocator();
    for (auto&& v : values) {
      arr.PushBack(v, allocator);
    }
    rapidjson::Pointer(token).Set(*this, arr, allocator);
    return 0;
  }

  template <typename T>
  int pushBack(const std::vector<std::string>& keys, const T& value) {
    if (keys.empty()) {
      return -1;
    }
    if (!this->IsObject()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }

    auto valuePtr = rapidjson::Pointer(token).Get(*this);
    if (valuePtr == nullptr || !valuePtr->IsArray()) {
      return -1;
    }
    auto& allocator = this->GetAllocator();
    valuePtr->PushBack<T>(value, allocator);
    return 0;
  }
  int pushBack(const std::vector<std::string>& keys, rapidjson::Value& value) {
    if (keys.empty()) {
      return -1;
    }
    if (!this->IsObject()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }

    auto valuePtr = rapidjson::Pointer(token).Get(*this);
    if (valuePtr == nullptr || !valuePtr->IsArray()) {
      return -1;
    }
    auto& allocator = this->GetAllocator();
    valuePtr->PushBack(value, allocator);
    return 0;
  }
  int popBack(const std::vector<std::string>& keys) {
    if (keys.empty()) {
      return -1;
    }
    std::string token;
    for (auto&& key : keys) {
      token += "/" + key;
    }
    auto valuePtr = rapidjson::Pointer(token).Get(*this);
    if (valuePtr == nullptr || !valuePtr->IsArray()) {
      return -1;
    }
    valuePtr->PopBack();
    return 0;
  }
};