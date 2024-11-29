var data =
{
  hora: "00:00 AM",
  fecha: "00/00/0000",
  temperatura: 0,
  presion: 0,
  altitud: 0,
  humedad: 0,
  mq: 0
};

async function fetchData()
{
  try
  {
    const response = await fetch('/mqtt'); // Hacer la petición al endpoint
    const res = await response.json();        // Parsear la respuesta como JSON
    console.log("Datos recibidos de MQTT:", res); // Mostrar en la consola
    data = res;
    Load();
  }
  catch (error)
  {
    console.error("Error al obtener datos:", error);
  }
}

  // Llamar a fetchData cada 2 segundos
  setInterval(fetchData, 2000);

/*function Load()
{
    document.getElementById("hora").innerHTML = data.hora;
    document.getElementById("fecha").innerHTML = data.fecha;
    document.getElementById("temperatura").innerHTML = "Temperatura: " + data.temperatura + "°C";
    document.getElementById("presion").innerHTML = "Presión: " + data.presion + " hPa";
    document.getElementById("altitud").innerHTML = "Altitud: " + data.altitud + " m";
    document.getElementById("humedad").innerHTML = "Humedad: " + data.humedad + "%";
    document.getElementById("mq").innerHTML = "MQ: " + data.mq + "%";
}*/

function Load()
{
    document.getElementById("hora").innerHTML = data.hora;
    document.getElementById("fecha").innerHTML = data.fecha;
    document.getElementById("temperatura").innerHTML = data.temperatura + "°C";
    document.getElementById("presion").innerHTML = data.presion + " hPa";
    document.getElementById("altitud").innerHTML = data.altitud + " m";
    document.getElementById("humedad").innerHTML = data.humedad + "%";
    document.getElementById("mq").innerHTML = data.mq + "%";
}