#ifndef MQTTDEVICE_H
#define MQTTDEVICE_H

#include <QObject>
#include <QMqttClient>
#include <QMqttTopicFilter>
#include <QTableWidget>
#include <QTimer>
#include <QFile>
#include <QTextStream>

extern QVector <QString> shapeIdVector;

class MqttDevice : public QMqttClient
{
    Q_OBJECT
    Q_ENUM()
public:
    explicit MqttDevice(QWidget *parent = nullptr);
    ~MqttDevice();
    void setParam(QString name, QString imei, QString imsi, QString iccid, QString msisdn);
    bool isAuthentified();

public:
    enum Event {
        Connected = 0,
        Disconnected,
        Subscribed,
        Unsubscribed,
        Received,
        Published
    };

private:
    QString m_deviceName, m_cliendId, m_deviceId;
    QString m_imei, m_imsi, m_iccid, m_msisdn;
    QChar m_sendFreq;

    bool m_authentification;

    QMqttTopicName *m_authDeviceTopic, *m_dataTopic, *m_statusTopic;
    QMqttTopicFilter *m_authSysTopic, *m_commandTopic;

    QVector <QTableWidgetItem*> eventTablebWidgetItemVector;
    QVector <QTableWidgetItem*> topicTablebWidgetItemVector;

    QTimer *deviceTimer;
    QFile *fileTrajects;
    QTextStream *textStreamTrajects;
    QString shapeId;
    QString *firstLine;
    qint64 posFirstLine;

    void tableAddInfo(QString Event, QString topicName = "", QString msg = "");

public slots:
    void Authentifaction(void);
    void receivedMsgHandle(const QByteArray &message, const QMqttTopicName &topic);
    void sendData();
    void deviceDisconnect();


signals:

};

#endif // MQTTDEVICE_H
