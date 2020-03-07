import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    id: itemLogin
    property string userSigned

    anchors.fill: parent

    QWebScraper {
        id: scraper
        actions: [
            QWebScraperAction {
                endpoint: "https://www.fifaindex.com/accounts/login/"
                scraps: [
                    {
                        "name": "token",
                        "query": "/html/body/main/div/div[2]/div/div/div/form/input/@value/string()"
                    }
                ]
            },
            QWebScraperAction {
                endpoint: "https://www.fifaindex.com/accounts/login/"
                method: "POST"
                headers: {"referer": "https://www.fifaindex.com/accounts/login/"}
                data: [
                    {"csrfmiddlewaretoken": "%%token%%"},
                    {"login": usernameText.text},
                    {"password": passwordText.text}
                ]
                validator: {
                    "name": "username",
                    "query": "/html/body/main/div/script/string()"
                }
            }
        ]
        Component.onCompleted: console.log(scraper.actions[1].valid)
    }

    Column {
        anchors.centerIn: parent
        spacing: 10
        visible: !scraperIndicator.running

        Column {
            spacing: 5

            TextInput {
                id: usernameText
                cursorVisible: true
                width: 100
            }

            TextInput {
                id: passwordText
                cursorVisible: true
                width: 100
            }

            Button {
                id: loginButton
                text: userLabel.visible ? "OK" : "Login"
                onClicked: scraper.scrap()
            }
        }

        Label {
            id: userLabel
            color: "green"
            text: "You are now logged in"
            visible: scraper.actions[1].valid
        }
    }

    BusyIndicator {
        id: scraperIndicator
        width: 150
        height: 150
        anchors.centerIn: parent
        running: scraper.status === QWebScraperStatus.Loading
    }
}
