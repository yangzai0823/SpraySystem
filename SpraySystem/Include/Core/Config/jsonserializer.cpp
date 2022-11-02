#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDir>
#include <iostream>
#include <QtDebug>
#include "Include/Core/Config/jsonserializer.h"

namespace vws {
    namespace properties {
        using Config = vws::properties::Config;

        int fromJson_(Config& config, const QJsonValue& jv) {
            if (jv.isObject()) {
                QJsonObject ob = jv.toObject();
                for (auto it = ob.begin(); it != ob.end(); ++it) {
                    Config child = config.mapMakeChild(it.key());
                    fromJson_(child, it.value());
                }
            }
            else if (jv.isArray()) {
                QJsonArray a = jv.toArray();
                for (auto it = a.begin(); it != a.end(); ++it) {
                    Config child = config.listAppendNew();
                    fromJson_(child, *it);
                }
            }
            else {
                if (jv.isString()) {
                    config.setValue(jv.toString());
                }
                else if (jv.isDouble()) {
                    config.setValue(jv.toDouble());
                }
                else if (jv.isBool()) {
                    config.setValue(jv.toBool());
                }
            }
            return 0;
        }


        int JsonSerializer::fromJson(Config& config, std::string filename) {

            QFile loadFile(QString::fromStdString(filename));

            if (!loadFile.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open file.");
                return -1;
            }
            QByteArray allData = loadFile.readAll();
            loadFile.close();

            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
            if (json_error.error != QJsonParseError::NoError)
            {
                qDebug() << "json error!";
                return -1;
            }
            QJsonValue jv;
            if (jsonDoc.isObject()) {
                jv = jsonDoc.object();
            }
            else {
                jv = jsonDoc.array();
            }
            fromJson_(config, jv);
            return 0;
        }

        int toJson_(const Config& config, QJsonValue& jv) {
            switch (config.getType()) {
            case Config::List:
            {
                QJsonArray ja;
                for (int i = 0; i < config.listLength(); i++) {
                    QJsonValue jvc;
                    toJson_(config.listChildAt(i), jvc);
                    ja.append(jvc);
                }
                jv = ja;
                break;
            }
            case Config::Map:
            {
                QJsonObject job;
                Config::MapIterator map_iter = config.mapIterator();
                while (map_iter.isValid()) {
                    Config child = map_iter.currentChild();
                    QString key = map_iter.currentKey();
                    QJsonValue v;
                    toJson_(child, v);
                    job[key] = v;

                    map_iter.advance();
                }
                jv = job;
                break;
            }
            case Config::Value:
            {
                QString value = config.getValue().toString();
                jv = value;
                break;
            }
            default:
                break;
            }
            return 0;
        }

 
        int JsonSerializer::toJson(const Config& config, const std::string filename) {
            QJsonValue jv;
            toJson_(config, jv);
            QJsonDocument jd;
            if (jv.type() == QJsonValue::Array) {
                jd.setArray(jv.toArray());
            }
            else if (jv.type() == QJsonValue::Object) {
                jd.setObject(jv.toObject());
            }

            QByteArray ba = jd.toJson();

            QFile saveFile(QString::fromStdString(filename));

            if (!saveFile.open(QIODevice::WriteOnly)) {
                qWarning("Couldn't open file.");
                return -1;
            }
            saveFile.write(ba);
            saveFile.close();

            return 0;
        }

    }
}
