import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    id: itemListElements
    anchors.fill: parent
    property var topPlayers: []

    QWebScraper {
        id: scraper        
        actions: [
            {
                "endpoint": "https://www.fifaindex.com/players/top/",                
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
                topPlayers = ctx["topPlayers"]
                console.log(JSON.stringify(topPlayers));
            }
        }
    }

    Column {
        spacing: 10
        visible: !scraperIndicator.running

        Button {
            id: getTopPlayerButton
            text: "Scrap"
            onClicked: scraper.scrap()
        }

        ListView {
            id: listView
            width: 180; height:  count > 0 ? contentHeight : 0
            model: scraper.ctx["topPlayers"]

            delegate: ItemDelegate {
                text: modelData.name + " - " + modelData.team
            }

            ScrollIndicator.vertical: ScrollIndicator { }
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
