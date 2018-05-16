//DICOM files upload page
import {Component} from '@angular/core';
import {RESTService} from '../../services/rest.service';
import { ChangeDetectorRef } from '@angular/core';
import { ChangeDetectionStrategy } from '@angular/core';


import { FileDropModule, UploadFile, UploadEvent } from 'ngx-file-drop/lib/ngx-drop';

@Component({
  selector:'upload',
  changeDetection: ChangeDetectionStrategy.OnPush,
  templateUrl: 'upload.component.html'
})

export class UploadComponent {
  myString: string[] = [];
  myReader = new FileReader();
  numberToUpload: number;
  numberUploaded: number;
  changeDetector: ChangeDetectorRef;

  constructor(private service: RESTService, private ref: ChangeDetectorRef){
    this.changeDetector = ref;
  }

  /*
    This function send the content of one or multiple files to the server
    Parameter:
      -inputValue: the element that generate the onfiledrop event. It contains
      the array of files to send to the server
  */
  toDoDragAndDrop(inputValue: any)
  {

    this.numberToUpload=inputValue.files.length;
    this.numberUploaded=0;
    for (let i=0 ; i < inputValue.files.length;i++) {
      inputValue.files[i].fileEntry.file(info => {
        this.readFiles(info, i)
          .then(_=>{this.service.upload(this.myString[i])
                      .then(_=>{this.numberUploaded++;
                                console.log(this.numberUploaded);
                                this.changeDetector.detectChanges();});});
      })
    }

  }

  /*
    This function send the content of one or multiple files to the server
    Parameter:
      -inputValue: the element that generate the 'change' event. It contains the
       array of files to send to the server
  */
  toDoButton(inputValue: any)
  {

    this.numberToUpload=inputValue.files.length;
    this.numberUploaded=0;
    for (let i=0 ; i < inputValue.files.length;i++){
      this.readFiles(inputValue.files[i], i)
        .then(_=>{this.service.upload(this.myString[i])
                    .then(_=>{this.numberUploaded++;
                              console.log(this.numberUploaded);
                              this.changeDetector.detectChanges();});});
    }
  }

  /*
    This function read the content of the File passed as argument and put it in
    a string array
    Parameters:
      -file: The File object reprensetinf the file to read
      -index: the index in the string array where the content must be store
  */
  readFiles(file:File, indice: number){
    let promise= new Promise((resolve, reject) =>{
      let myReader = new FileReader();
      myReader.readAsArrayBuffer(file);
      myReader.onloadend = (e) => {
        this.myString[indice]= myReader.result;
        resolve();
      },
        msg => { // Error
          reject(msg);
        }
    });
    return promise;
  }

}
