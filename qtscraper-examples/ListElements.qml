import QtQuick 2.12
import QtQuick.Controls 2.13

import com.ifba.edu.scraping 1.0

Item {
    anchors.fill: parent

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
                    }
                ]
            }
        ]
        onStatusChanged: {
                if (scraper.status === QWebScraperStatus.Ready){
                    console.log(status);
                    console.log(JSON.stringify(ctx));
                }
        }
    }

    Column {

        Label {

        }

        Label {

        }


        Button {
            text: "Scrap"
            onClicked: {
                scraper.scrap()
            }
        }
    }

}
