import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    id: itemListElements
    anchors.fill: parent
    property var topPlayers

    QWebScraper {
        id: scraper
        url: "https://www.fifaindex.com/"
        actions: [
            {
                "endpoint": "https://www.fifaindex.com/players/top/",
                "method": "GET",
                "scraps": [                    
                    {
                        "name": "topPlayers",
                        "query": "/html/body/main/div/div[2]/div[2]/div[2]/table/tbody/tr/td[$index]/a/@title/string()",
                        "indexes": [4, 8],
                        "headers": ["name", "team"],
                        "responseParser": QWebScraperResponseParser.TableParser
                    }
                ]
            }
        ]
        onStatusChanged: {
            if (scraper.status === QWebScraperStatus.Ready) {
                var topPlayers = ctx["topPlayers"]
            }
        }
    }

    ListView {
        anchors.fill: parent
        model: topPlayers

        delegate: ItemDelegate {
            text: modelData.name + " - " + modelData.team
            width: parent.width
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    Button {
        id: getTopPlayerButton
        anchors.centerIn: parent
        text: "Scrap"
        onClicked: scraper.scrap()
    }

    BusyIndicator {
        id: scraperIndicator
        width: 150
        height: 150
        anchors.centerIn: parent
        running: scraper.status === QWebScraperStatus.Loading
    }    
}
