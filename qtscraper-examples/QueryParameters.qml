import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    id: itemQueryParameters
    property var playerObj

    anchors.fill: parent

    QWebScraper {
        id: scraper
        url: "https://www.fifaindex.com/"
        actions: [
            {
                "endpoint": "https://www.fifaindex.com/players/?name=" + searchArea.text + "&order=desc",
                "method": "GET",
                "scraps": [
                    {
                        "name": "name",
                        "query": "/html/body/main/div/div[2]/div[2]/div[2]/table/tbody/tr[3]/td[4]/a/string()"
                    }
                ]
            }
        ]
        onStatusChanged: {
            if (scraper.status === QWebScraperStatus.Ready) {
                itemQueryParameters.playerObj = scraper.ctx
            }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 10
        visible: !scraperIndicator.running

        Row {
            spacing: 5

            TextInput {
                id: searchArea
                cursorVisible: true
                width: 100
            }

            Button {
                id: searchButton
                text: "Search"
                onClicked: scraper.scrap()
            }
        }

        Label {
            text: playerObj ? playerObj.name[0] : "Not found!"
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
