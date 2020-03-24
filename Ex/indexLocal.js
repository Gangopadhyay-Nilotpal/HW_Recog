
const liveServer = require("live-server");
const fs = require('fs');

const fname = './user_keys.txt';
const jsonPath = "./user_keys_R.json"

console.log('='.repeat(106));
var array = fs.readFileSync(fname).toString().split("\n");

toJson = {
    "applicationKey": (array[0].split(": ")[1]).slice(1,-2),
    "hmacKey": (array[1].split(": ")[1]).slice(1,-1)
};

var jsonCont = JSON.stringify(toJson);
console.log(jsonCont);

fs.exists(jsonPath, (exists) => {
	if (exists === true) { 
	
		fs.unlink(jsonPath, (err) => {
          //if (err) throw err;
		  console.log('Old user_keys_R.json File - Deleted!');
	    });		
	}
	
	fs.writeFile(jsonPath, jsonCont, 'utf8', (err) => {	
        //if (err) throw err; 
        console.log("New user_keys_R.json File - Created.");
    });
});

console.log(' indexLocal running...');
console.log('='.repeat(106));

liveServer.start({"host": "localhost", "port": 3333, "open":"/pointer_events_workon.html"});