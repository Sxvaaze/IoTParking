const express = require('express');
const app = express();
const cors = require('cors');
const port = 8192;

app.use(cors());

app.get('/', (req, res) => {
    res.send(require('./mqtt_connect'));
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})