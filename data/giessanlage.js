async function sendRequest(endpoint) {
    var resp = await fetch(endpoint);
    if (resp.status != 200) return "0";
    return await resp.text();
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", endpoint, false ); // false for synchronous request
    xmlHttp.send( null );
    if (xmlHttp.status != 200) return "0";
    return xmlHttp.responseText;
}

window.addEventListener('load', async function() {
    document.getElementById("endpoints").value = await sendRequest("/wateringPoints");
    //document.getElementById("endpoints").oninput();
    document.getElementById("mlPerEndpoint").value = await sendRequest("/baseMlPerWateringPoint");
    //document.getElementById("mlPerEndpoint").oninput();
    document.getElementById("mainPumpSpeed").value = await sendRequest("/mainPumpSpeed");
    document.getElementById("mainPumpSpeed").oninput();
    document.getElementById("okSoilMoisture").value = await sendRequest("/okSoilMoisture");
    document.getElementById("okSoilMoisture").oninput();

    document.getElementById("literControlRange").oninput();

    document.getElementById("geplanteBewasserungsmenge").value = await sendRequest("/autoWaterToday");

    setTimeout(addDayLog, 5000);
});

async function sendSettings() {
    await sendRequest("/wateringPoints/" + document.getElementById("endpoints").value);
    await sendRequest("/baseMlPerWateringPoint/" + document.getElementById("mlPerEndpoint").value);
    await sendRequest("/mainPumpSpeed/" + document.getElementById("mainPumpSpeed").value);
    await sendRequest("/okSoilMoisture/" + document.getElementById("okSoilMoisture").value);
}

async function water(liter) {
    await sendRequest("/water/" + liter);
}

async function addDayLog() {
    var timestamps = [];
    var soil = [];
    var lux = [];
    for (var i = 1; i <= 48 ; i++) {
        var percentage = i/48 * 100;
        document.getElementById("lineChartDayLoader").setAttribute("aria-valuenow", percentage);
        document.getElementById("lineChartDayLoader").style.width = percentage + "%";
        var data = JSON.parse(await sendRequest("/log/"+i));
        if (data.time == 0) break;
        var date = new Date(data.time);
        timestamps.unshift(date.getDay() + "." + (date.getMonth()) + 1 + "." + date.getFullYear() + " " + date.getHours() + ":" + date.getMinutes());
        soil.unshift(data.soil);
        lux.unshift(data.lux);
    }
    document.getElementById("lineChartDayLoaderContainer").style.display = "none";
    var ctxL = document.getElementById("lineChartDay").getContext('2d');
    drawChart(ctxL, timestamps, soil, lux);
}

/////// CHART
function drawChart(canvas, labels, soil, lux){
    //line
    return new Chart(canvas, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [{
        label: "Bodenfeuchtigkeit",
        data: soil,
        yAxisID: 'B',
        backgroundColor: [
            'rgba(105, 0, 132, .2)',
        ],
        borderColor: [
            'rgba(200, 99, 132, .7)',
        ],
        borderWidth: 2
        },
        {
        label: "Helligkeit in lux",
        data: lux,
        yAxisID: 'A',
        backgroundColor: [
            'rgba(0, 137, 132, .2)',
        ],
        borderColor: [
            'rgba(0, 10, 130, .7)',
        ],
        borderWidth: 2
        }
        ]
    },
    options: {
        responsive: true,
        scales: {
            yAxes: [{
                id: 'A',
                type: 'linear',
                position: 'left',
                ticks: {
                    callback: function(label, index, labels) {
                        return label;
                    }
                },
            }, {
              id: 'B',
              type: 'linear',
              position: 'right',
            }]
          }
    }
    });
}