import {Component, Input, OnInit} from '@angular/core';
import {ActivatedRoute, Router} from "@angular/router";

import {RESTService} from "../../services/rest.service";
import {PermissionsService} from "../../services/permissions.service";
import {AppComponent} from "../app/app.component";

@Component
({
  selector: 'query-page',
  templateUrl: 'query.component.html',
  providers: [RESTService]
})

export class QueryComponent implements OnInit
{

  studyModalities = ['CR', 'CT', 'MR', 'NM', 'PT', 'US', 'XA'];
  nbCheckedModalities = 0;
  checkedModalities = [false, false, false, false, false, false, false];
  @Input() selectedModality: string;
  modalities: string;

  level = 'Study';

  accessionNumber = '';
  patientName = '';
  patientBirthDate = '';
  patientID = '';
  patientSex =  '';
  studyDate = '';
  studyDescription = '';
  modalitiesInStudy = ['*'];

  queryID: string;

  constructor(private permissions: PermissionsService, private service: RESTService, private router: Router, private app: AppComponent){}

  ngOnInit(): void
  {
    this.service.getModalities().then(res =>
    {
      this.modalities = res.json()
        .map(modality => {
          return modality.name;
        });
      this.selectedModality = this.modalities[0];
    });

  }

  /*
   * Check the checkbox of the study modality
   */
  checkModality(index: number): void
  {
    if(this.checkedModalities[index] == true)
      this.nbCheckedModalities--;
    else
      this.nbCheckedModalities++;
    this.checkedModalities[index] = !this.checkedModalities[index];
  }

  /*
   * Select the remote modality where to query from
   */
  selectModality(modality: string): void
  {
    this.selectedModality = modality;
  }

  /*
   * Validate the fields of the search and proceed to the query
   */
  ok(): void {
    if (this.accessionNumber == '')
      this.accessionNumber = '*';
    else
      this.accessionNumber = '*' + this.accessionNumber + '*';

    if (this.patientName == '')
      this.patientName = '*';
    else
      this.patientName = '*' + this.patientName + '*';

    if (this.patientID == '')
      this.patientID = '*';
    else
      this.patientID = '*' + this.patientID + '*';

    if (this.patientSex == '')
      this.patientSex = '*';
    else
      this.patientSex = '*' + this.patientSex + '*';

    if (this.patientBirthDate == '')
      this.patientBirthDate = '*';
    else
      this.patientBirthDate = '*' + this.patientBirthDate + '*';

    if (this.studyDescription == '')
      this.studyDescription = '*';
    else
      this.studyDescription = '*' + this.studyDescription + '*';

    if (this.studyDate == '')
      this.studyDate = '*';
    else
      this.studyDate = '*' + this.studyDate + '*';

    if(this.nbCheckedModalities > 0)
    {
      this.modalitiesInStudy = new Array<string>(this.nbCheckedModalities);
      let j = 0;
      for(let i = 0; i < this.studyModalities.length; i++)
      {
        if( this.checkedModalities[i] == true)
        {
          this.modalitiesInStudy[j] = this.studyModalities[i];
          j++;
        }
      }
      console.log(this.modalitiesInStudy);
    }

    this.service.getQueryID(this.selectedModality,
                                    this.level,
                                    this.accessionNumber,
                                    this.patientBirthDate,
                                    this.patientID,
                                    this.patientName,
                                    this.patientSex,
                                    this.studyDate,
                                    this.studyDescription,
                                    this.modalitiesInStudy).then(res =>
      {
        this.queryID = res.json().ID;
        this.app.toastJobResult(true, 'query', '/retrieve', this.queryID);
        this.router.navigate(['/retrieve', {id: this.queryID}]);
      }, ()=> this.app.toastJobResult(false, 'query'));
    this.resetFields();
    this.app.toastInfo('jobRegistered');
  }

  /*
  Reset the fields of the research panel.
   */
  resetFields()
  {
    this.accessionNumber = '';
    this.patientName = '';
    this.patientBirthDate = '';
    this.patientID = '';
    this.patientSex =  '';
    this.studyDate = '';
    this.studyDescription = '';
    this.modalitiesInStudy = ['*'];

    this.nbCheckedModalities = 0;
    this.checkedModalities = [false, false, false, false, false, false, false];
  }
}
