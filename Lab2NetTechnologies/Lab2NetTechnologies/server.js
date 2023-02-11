express = require("express");
const app = express();
const bodyParser = require("body-parser");
const multer = require("multer");

//cb - callback

function changeName(Name) {
    return Buffer.from(Name, 'latin1').toString(
        'utf8');
}

const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, './upload/')
    },
    filename: function (req, file, cb) {
        cb(null, changeName(file.originalname));
    }
});

var upload = multer({ storage: storage });

const { Client } = require('pg');

const client = new Client({
    user: 'postgres',
    host: 'localhost',
    database: 'net_tech',
    password: '1234',
    port: 5432,
});

client.connect();
// deleting: delete from records where id_rec in $1;

//header, marks, text, files
const insert_q = "insert into records (header,marks,text,files) values($1, $2, $3, $4);";
//const find_header_q
const find_header_q = "select * from records where header like $1;";
//const find_marks_q
const find_marks_q = "select * from records where marks like $1;";
//const find_text_q
const find_text_q = "select * from records where text like $1;";

//use __dirname + upload\\

app.use(bodyParser.urlencoded({ extended: true })); // get objects also from url
app.use(bodyParser.json()); // data, got from body, should be in json

app.use(express.static('public'));
app.use(express.static('upload'));

app.set('view engine', 'ejs');

function set_file_handler(req, res) {
    try {
        console.log(req.body);
        console.log(req.files);
        var header = req.body.header;
        var marks = ' ' + req.body.marks + ' ';
        var text = req.body.text;
        var files = req.files;
        for (let i = 0; i < files.length; i++) {
            files[i] = files[i].filename;
        }
        client.query(insert_q, [header, marks, text, files])
            .then(res => {
                console.log("We got data successfully");
            })
            .catch(err => {
                console.error(err);
            });
        res.sendFile(__dirname + "/public/success.html");
    }
    catch (err) {
        res.sendFile(__dirname + "/public/error.html");
        console.log(err);
    }
}
app.post("/create", upload.array("files"), set_file_handler);
//let sum = (a, b) => a + b;
/* Эта стрелочная функция представляет собой более короткую форму:
let sum = function(a, b) {
  return a + b;
};*/
function find_header_handler(req, res)
{
    try {
        var header = '%' + req.body.header + '%';
        client.query(find_header_q, [header])
            .then(result => {
                console.log("We found data successfully from handler");
                console.log(JSON.stringify(result.rows));
                res.render('found', result);
            })
            .catch(err => {
                console.error(err);
            });
    }
    catch (err) {
        res.sendFile(__dirname + "/public/error.html");
        console.log(err);
    }
    //res.sendFile(__dirname + "/public/index.html");
}

function find_marks_handler(req, res) {
    try {
        var marks = '% ' + req.body.marks + ' %';
        console.log(marks);
        client.query(find_marks_q, [marks])
            .then(result => {
                console.log("We found data successfully from marks");
                console.log(JSON.stringify(result.rows));
                res.render('found', result);
            })
            .catch(err => {
                console.error(err);
            });
    }
    catch (err) {
        res.sendFile(__dirname + "/public/error.html");
        console.log(err);
    }
   // res.sendFile(__dirname + "/public/index.html");
}

function find_text_handler(req, res) {
    try {
        var text = '%' + req.body._text + '%';
        console.log(text);
        client.query(find_text_q, [text])
            .then(result => {
                console.log("We found data successfully from text");
                console.log(JSON.stringify(result.rows));
                res.render('found', result);
            })
            .catch(err => {
                console.error(err);
            });
    }
    catch (err) {
        res.sendFile(__dirname + "/public/error.html");
        console.log(err);
    }
   // res.sendFile(__dirname + "/public/index.html");
}

app.post("/find_header", find_header_handler);
app.post("/find_marks", find_marks_handler);
app.post("/find_text", find_text_handler);

app.listen(1337);

