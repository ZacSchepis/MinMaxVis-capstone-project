//
// Created by matth on 4/12/2025.
//

#ifndef GEMINICLIENT_H
#define GEMINICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GeminiClient : public QObject {
    Q_OBJECT

public:
    explicit GeminiClient(QObject *parent = nullptr);
    void askGemini(const QString &prompt);

    signals:
        void responseReady(const QString &response);

    private slots:
        void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey;
};
#endif //GEMINICLIENT_H



