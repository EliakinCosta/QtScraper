import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    id: itemLogin
    property string userSigned

    anchors.fill: parent

    QWebScraper {
        id: scraper
        url: "https://www.fifaindex.com/"
        actions: [
            {
                "endpoint": "https://www.fifaindex.com/accounts/login/",
                "method": "GET",
                "scraps": [
                    {
                        "name": "token",
                        "query": "/html/body/main/div/div[2]/div/div/div/form/input/@value/string()"
                    }
                ]
            },
            {
                "endpoint": "https://www.fifaindex.com/accounts/login/",
                "method": "POST",
                "headers": {"referer": "https://www.fifaindex.com/accounts/login/"},
                "data": [
                    {"csrfmiddlewaretoken": "%%token%%"},
                    {"login": usernameText.text},
                    {"password": passwordText.text}
                ],
                "validator": {
                    "name": "username",
                    "query": "/html/body/main/div/script/string()"
                }
            }
        ]
        onStatusChanged: {
            if (scraper.status === QWebScraperStatus.Ready) {
                if (scraper.ctx["username"][0].toString().indexOf(usernameText.text) !== -1){
                    itemLogin.userSigned = usernameText.text
                }

            }
        }
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
            text: userSigned ? userSigned : ""
            visible: userSigned
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
