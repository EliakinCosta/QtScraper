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
                        "name": "title",
                        "query": "/html/head/title/string()"
                    },
                    {
                        "name": "names",
                        "query": "/html/body/main/div/div[2]/div[2]/div[2]/table/tbody/tr/td[4]/a/@title/string()"
                    },
                    {
                        "name": "overalls",
                        "query": "/html/body/main/div/div[2]/div[2]/div[2]/table/tbody/tr/td[3]/span[1]/string()"
                    }
                ]
            }
        ]
        onStatusChanged: {
            if (scraper.status === QWebScraperStatus.Ready) {
                var names = scraper.ctx.names;
                var overalls = scraper.ctx.overalls;
                topPlayers = arrays_as_objects(names, overalls);
            }
        }
    }

    ListView {
        anchors.fill: parent
        model: topPlayers

        delegate: ItemDelegate {
            text: modelData.name + " - " + modelData.overall
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

    function arrays_as_objects(array1, array2) {
        var final_array = [];

        for (var index=0; index<array1.length; index++) {
            var obj = {};
            obj["name"] = array1[index];
            obj["overall"] = array2[index];
            final_array.push(obj);
        }
        return final_array;
    }
}
