const chokidar = require('chokidar');
const fsExtra = require('fs-extra');
const rimraf = require('rimraf');
const path = require('path');
const fs = require('fs');

const OTFstor = './OTF_Files';
const OTFfile = './OTF_File.txt';
//const starting_part = "{ \"events\": [";
//const ending_part = "]}";


if (fs.existsSync(OTFstor)){ 
	fsExtra.emptyDirSync(OTFstor);
	console.log(' Old OTF files are Deleted from "OTF_Files" folder..');
}

if (!fs.existsSync(OTFstor)){ 
	fs.mkdirSync(OTFstor);
	console.log(' New "OTF_Files" folder has created..');
}

fs.exists(OTFfile, (exists) => {
	if (exists === true) { 	
		fs.unlink(OTFfile, (err) => {
          //if (err) throw err;
		  console.log(' Old OTF_File.txt - Deleted!!');
	    });
	}
});


// One-liner for current directory
chokidar.watch(OTFstor).on('add', (filepath, filestatus) => {
	//console.log(filepath);
	fs.exists(OTFfile, (exists) => {
		if (exists === true) {
			fs.unlink(OTFfile, (err) => {
			  //if (err) throw err;
			});
		}
	});	
	
	//---------------------------------------------------------
	fs.readdir(OTFstor, (err, files) => {
		//if (err) throw err;		
		var file_count = files.length;
		var incre = 0;
		//---------------------------------------------------------
		files.forEach((file) => {
			incre++;
			
			if (incre === 1) {
				fs.writeFileSync(OTFfile, "{ \"events\": [", {flag:'a'});
			}
			
			if (incre < file_count) {
				fs.readFile(OTFstor.concat("/", file), (err, data) => {
					//if (err) throw err; 
					var file_not_end = data.toString();
					fs.writeFileSync(OTFfile, file_not_end.concat(","), {flag:'a'});
				});
			}
			
			if(incre === file_count) {
				fs.readFile(OTFstor.concat("/", file), (err, data) => {
					//if (err) throw err; 
					var file_end = data.toString();
					fs.writeFileSync(OTFfile, file_end.concat("]}"), {flag:'a'});
					console.log(' OTF_File.txt updated..');
				});
			}			
		});
		//---------------------------------------------------------
	});
	//---------------------------------------------------------
});