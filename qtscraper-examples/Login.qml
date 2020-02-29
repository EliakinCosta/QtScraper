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
                "data": [
                    {"csrfmiddlewaretoken": "%%token%%"},
                    {"login": usernameText.text},
                    {"password": passwordText.text}
                ],
                "scraps": [
                    {
                        "name": "name",
                        "query": "/html/body/header/nav/div/ul[2]/li/a/span/string()"
                    }
                ]
            }
        ]
        onStatusChanged: {
            if (scraper.status === QWebScraperStatus.Ready) {
                console.table(scraper.ctx);
                itemLogin.userSigned = scraper.ctx["name"][0];
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
