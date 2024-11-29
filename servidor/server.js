const express = require('express');
const mqtt = require('mqtt');

const PORT = 3000;

const app = express();
const client = mqtt.connect("mqtt://test.mosquitto.org");

var obj = {
    hora: "00:00 AM",
    fecha: "22/11/2024",
    temperatura: 0,
    presion: 0,
    altitud: 0,
    humedad: 0,
    mq: 0,
};

client.on('connect', function ()
{
    client.subscribe('eslitop', function (err)
    {
        if (!err)
        {
            console.log('Suscripción exitosa al topic "eslitop"');
        }
    });
});
client.on("message", (topic, message) =>
{
    console.log(message.toString());
    obj = JSON.parse(message.toString());
});

app.get('/mqtt', (req, res) => {
    res.json(obj);
  });
  
  // Servir archivos estáticos desde la carpeta "public"
  app.use(express.static('public'));
  
  // Iniciar el servidor HTTP
  app.listen(PORT, () => {
    console.log(`Servidor escuchando en http://localhost:${PORT}`);
  });