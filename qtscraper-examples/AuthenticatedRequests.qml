import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    id: itemLogin

    anchors.fill: parent

    QWebScraper {
        id: scraperLogin
        keepAlive: true
        actions: [
            QWebScraperAction {
                endpoint: "https://talentos.carreirarh.com/site/user_login/"
                scraps: [
                    {
                        "name": "token",
                        "query": "/html/body/div/div/div[2]/div/form/input/@value/string()"
                    }
                ]
            },
            QWebScraperAction {
                endpoint: "https://talentos.carreirarh.com/site/user_login/"
                method: "POST"
                headers: {"referer": "https://talentos.carreirarh.com/site/user_login/"}
                data: [
                    {"csrfmiddlewaretoken": "%%token%%"},
                    {"login": usernameText.text},
                    {"password": passwordText.text}
                ]
                validator: {
                    "name": "email",
                    "query": "/html/body/div[2]/nav/div/ul/li[1]/div[1]/a/span/span[2]/string()"
                }
            }
        ]
    }

    QWebScraper {
        id: scrapGetAuth
        actions: [
            QWebScraperAction {
                endpoint: "https://talentos.carreirarh.com/site/profile/notifications/"
                scraps: [
                    {
                        "name": "email",
                        "query": "/html/body/div[2]/nav/div/ul/li[1]/div[1]/a/span/span[2]/string()"
                    }
                ]
            }
        ]
        onStatusChanged: {
            if (scrapGetAuth.status === QWebScraperStatus.Ready){
                console.log(JSON.stringify(scrapGetAuth.ctx))
                if (scrapGetAuth.ctx["email"] === emailText.text){
                    requestsLabel.color = "green"
                    requestsLabel.text = "OK"
                } else {
                    requestsLabel.color = "red"
                    requestsLabel.text = "User is not authenticated"
                }
            } else if (scrapGetAuth.status === QWebScraperStatus.Error){
                requestsLabel.color = "red"
                requestsLabel.text = "Error"
            }
        }
    }

    QWebScraper {
        id: scrapPostAuth
        actions: [
            QWebScraperAction {
                endpoint: "https://talentos.carreirarh.com/site/profile/notifications/"
                scraps: [
                    {
                        "name": "token",
                        "query": "/html/body/div[2]/div/div[2]/div/div[1]/div[2]/div/div/form/input/@value/string()"
                    }
                ]
            },
            QWebScraperAction {
                endpoint: "https://talentos.carreirarh.com/site/user_login/"
                method: "POST"
                headers: {"referer": "https://talentos.carreirarh.com/site/user_login/"}
                data: [
                    {"csrfmiddlewaretoken": "%%token%%"}
                ]
                validator: {
                    "name": "message",
                    "query": "/html/body/div[2]/div/div[2]/div/div[1]/div[2]/div/div/div/string()"
                }
            }
        ]

        onStatusChanged: {
            if (scrapPostAuth.status === QWebScraperStatus.Ready){
                console.log(JSON.stringify(scrapPostAuth.ctx))
                requestsLabel.color = "green"
                requestsLabel.text = "OK"
            } else if (scrapGetAuth.status === QWebScraperStatus.Error){
                requestsLabel.color = "red"
                requestsLabel.text = "User is not authenticated"
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
                id: emailText
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
                text: "Login"
                onClicked: {
                    scraperLogin.clearCookies();
                    scraperLogin.scrap();
                }
            }

            Button {
                id: scrapGetAuthButton
                text: "GET"
                onClicked: scraperGetAuth.scrap()
            }

            Button {
                id: scrapPostAuthButton
                text: "POST"
                onClicked: scrapPostAuth.scrap()
            }
        }

        Label {
            id: userLabel
            color: "green"
            text: "You are now logged in"
            visible: scraperLogin.actions[1].valid
        }

        Label {
            id: requestsLabel
        }
    }

    BusyIndicator {
        id: scraperIndicator
        width: 150
        height: 150
        anchors.centerIn: parent
        running: scraperLogin.status === QWebScraperStatus.Loading || scrapGetAuth.status === QWebScraperStatus.Loading
    }
}
