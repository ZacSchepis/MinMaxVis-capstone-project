#include "GeminiClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QDebug>

GeminiClient::GeminiClient(QObject *parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &GeminiClient::onReplyFinished);
    apiKey = "AIzaSyAOQH4y8v1XEjPbleaO7Mhx_myxlqvSVls";  // Replace with your Gemini API key
}

void GeminiClient::askGemini(const QString &prompt) {
    QUrl url("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + apiKey);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Build correct JSON
    QJsonObject message{
            { "role", "user" },
            { "parts", QJsonArray{ QJsonObject{{"text", prompt}} } }
    };

    QJsonObject content{
            { "contents", QJsonArray{ message } }
    };

    QJsonDocument doc(content);
    QByteArray data = doc.toJson();

    qDebug() << "Sending Gemini request JSON:\n" << doc.toJson(QJsonDocument::Indented);

    networkManager->post(request, data);
}


void GeminiClient::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit responseReady("Error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);

    QString result = "No valid response from Gemini.";
    if (doc.isObject()) {
        QJsonObject root = doc.object();
        auto candidates = root["candidates"].toArray();
        if (!candidates.isEmpty()) {
            QJsonObject first = candidates[0].toObject();
            if (first.contains("content")) {
                QJsonObject content = first["content"].toObject();
                auto parts = content["parts"].toArray();
                if (!parts.isEmpty() && parts[0].toObject().contains("text")) {
                    result = parts[0].toObject()["text"].toString();
                }
            }
        }
    }

    emit responseReady(result);
    reply->deleteLater();
}
