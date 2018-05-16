import {Component, OnInit} from '@angular/core';
import {ActivatedRoute, Router} from "@angular/router";

import {Answer, System} from "../../services/miscellaneous.classes";
import {RESTService} from "../../services/rest.service";
import {PermissionsService} from "../../services/permissions.service";
import {AppComponent} from "../app/app.component";

@Component
({
  selector: 'retrieve-page',
  templateUrl: 'retrieve.component.html',
  providers: [RESTService]
})

export class RetrieveComponent implements OnInit
{
  queryID: string;
  nbResults: number;
  correct = false;

  checkedAnswers: Array<boolean>;
  numberSelected = 0;
  answers: Array<Answer>;
  system: System;


  constructor(private permissions: PermissionsService, private service: RESTService, private router: Router, private route: ActivatedRoute, private app: AppComponent){}

  ngOnInit(): void
  {
    this.route.params.subscribe(params => {
      if (params['id'])
      {
        this.correct = true;
        console.log(params);
        this.queryID = params['id'];
        this.service.getSystem().then(res => this.system = new System(res.json()));
        this.service.getQueryAnswers(this.queryID).then(res =>
        {
          this.nbResults = res.json().length;
          if(this.nbResults <= 0)
          {
            this.app.toastInfo('noResult');
          }
          else
            this.getAnswers();
        });
      }
    });
  }

  /*
   * Qet the answers of the query from the server
   */
  getAnswers(): void
  {
    this.answers = new Array<Answer>(this.nbResults);
    this.checkedAnswers = new Array<boolean>(this.nbResults);
    console.log(this.nbResults);
    console.log("Array is of size " + this.answers.length);
    let j = 0;

    for(let i = 0; i < this.nbResults; i++)
    {
      console.log("Getting answer of index"+i);
      this.service.getAnswers(this.queryID, i)
        .then( res => {
          this.answers[i] = new Answer(res.json());
          console.log(this.answers[i]);
        });
      this.checkedAnswers[i] = false;
      j++;
    }
  }

  /*
   * Retrieve a single study
   */
  retrieve(i: number): void
  {
    this.service.retrieveAnswer(this.queryID, this.system.DicomAet, i).then(()=>{
      this.app.toastJobResult(true, 'Retrieve : '+this.answers[i].PatientName)
    }, ()=> this.app.toastJobResult(false, 'Retrieve : '+this.answers[i].PatientName));
  }

  /*
   * retrieve all the studies returned from the query
   */
  retrieveAll(): void
  {
    this.service.retrieveAnswer(this.queryID, this.system.DicomAet).then(()=>
    {
      this.app.toastJobResult(true, 'retrieve all');
    }, ()=> this.app.toastJobResult(false, 'retrieve all'));
  }

  /*
   * Retrieve the selected studies
   */
  retrieveSelected(): void
  {
    for(let i = 0; i < this.nbResults; i++)
    {
      if(this.checkedAnswers[i] == true)
        this.retrieve(i);
    }
  }

  /*
  Check or uncheck an answer field depending on whether it has already been checked or not.
  PARAMETER:
    index: index of the field to be checked/unchecked
   */
  checkAnswer(index: number): void
  {
    this.checkedAnswers[index] = !this.checkedAnswers[index];
    if (this.checkedAnswers[index]) this.numberSelected++;
    else this.numberSelected--;
  }

}
