import {Component, Input, OnInit} from '@angular/core';
import {ActivatedRoute, Router} from "@angular/router";

import {ListComponent} from './list.component';
import {Patient, Statistics} from "../../services/miscellaneous.classes";
import {RESTService} from "../../services/rest.service";
import {stringDistance} from "codelyzer/util/utils";
import {PermissionsService} from "../../services/permissions.service";
import {AppComponent} from "../app/app.component";

@Component
({
  selector: 'patients-list',
  templateUrl: 'patients-list.component.html',
  providers: [RESTService]
})

export class PatientsListComponent extends ListComponent implements OnInit
{
  isSearch = false;
  searchIDs: Array<string>;

  patients: Patient[];
  tags = Object.keys;
  @Input() page: number;
  pageValues = new Array(1, 2, 3, 10, 20, 50);
  perPage = 10;
  prevPerPage = 10;
  totalPatients: number;
  totPage: number;
  statistics: Statistics;

  level = 'Patient';

  accessionNumber = '';
  patientName = '';
  patientBirthDate = '';
  patientID = '';
  patientSex =  '';
  studyDate = '';
  studyDescription = '';
  modalitiesInStudy = ['*'];

  access: string;
  name: string;
  ID: string;
  sex: string;
  birth: string;
  studyDesc: string;
  studyD: string;

  constructor(private permissions: PermissionsService, private service: RESTService, private router: Router, private app: AppComponent){super();}

  /*
  Called on the initialisation of the component.
   */
  ngOnInit(): void
  {
    this.totalPatients = 0;
    this.page = 1;
    this.searchIDs = new Array<string>(this.perPage);
    this.getStatistics();
  }

  /*
   * Retrieve 'nb' patients from the server, starting from the 'from'th patient
   */
  getElements(nb: number, from: number): void
  {
    if(!this.isSearch)
    {
      this.service.getPatients(nb, from)
        .then(res =>
        {
          switch (res.status)
          {
            case 403:
              this.permissions.errorRedirect();
              break;
            default:
              this.patients = res.json().map(patient => {
                return new Patient(patient);
              });
              if(this.patients.length <= 0)
              {
                this.app.toastInfo('noResult');
              }
          }
        })
    }
    else
    {
      this.service.getfilteredElement(
        this.level,
        this.access,
        this.birth,
        this.ID,
        this.name,
        this.sex,
        this.studyD,
        this.studyDesc,
        this.modalitiesInStudy,
        from, nb).then(res =>
      {
        switch (res.status)
        {
          case 403:
            this.permissions.errorRedirect();
            break;
          default:
            this.searchIDs = res.json();
            console.log(this.searchIDs);
            this.patients = new Array<Patient>(this.searchIDs.length);
            if(this.patients.length <= 0)
              this.app.toastInfo('noResult');
            for(let i = 0; i < this.patients.length; i++)
            {
              this.service.getPatientInfo(this.searchIDs[i])
                .then(res => this.patients[i] = new Patient(res.json()));
            }
        }

      })
    }
  }

  /*
   * Get the statistics from the server to extract the total number of patients
   */
  getStatistics(): void
  {
    this.service.getStatistics()
      .then(statistics => {
        this.statistics = statistics.json();
        this.totalPatients = this.statistics.CountPatients;
        this.totPage = Math.ceil(this.totalPatients / this.perPage);
        this.getElements(this.perPage,(this.page - 1) * this.perPage);});
  }

  /*
   * If the page number exceeds the number of total pages or if it is smaller than 1,
   * reset it to one of the border values.
   */
  borderPage(): void
  {
    if (this.page > this.totPage)
    {
      this.page = this.totPage;
    }
    if(this.page < 1)
    {
      this.page = 1;
    }
  }

  /*
   * Load the next page
   */
  nextPage(): void
  {
    if (this.page < this.totPage)
    {
      this.page ++;
    }
    this.requestPage();
  }

  /*
   * Load the previous page
   */
  prevPage(): void
  {
    if (this.page > 1)
    {
      this.page --;
    }
    this.requestPage();
  }

  /*
   * Request a given page
   */
  requestPage(): void
  {
    this.borderPage();
    this.getElements(this.perPage, (this.page-1) * this.perPage);
  }

  /*
   * Navigates to the selected patient page
   */
  onSelect(patientID: string): void
  {
    this.router.navigate(['/studies', {id: patientID}]);
  }

  /*
   * search a specific patient on the server
   */
  ok(): void
  {

    if (this.accessionNumber == '')
      this.access = '*';
    else
      this.access = '*' + this.accessionNumber + '*';

    if (this.patientName == '')
      this.name = '*';
    else
      this.name = '*' + this.patientName + '*';

    if (this.patientID == '')
      this.ID = '*';
    else
      this.ID = '*' + this.patientID + '*';

    if (this.patientSex == '')
      this.sex = '*';
    else
      this.sex = '*' + this.patientSex + '*';

    if (this.patientBirthDate == '')
      this.birth = '*';
    else
      this.birth = '*' + this.patientBirthDate + '*';

    if (this.studyDescription == '')
      this.studyDesc = '*';
    else
      this.studyDesc = '*' + this.studyDescription + '*';

    if (this.studyDate == '')
      this.studyD = '*';
    else
      this.studyD = '*' + this.studyDate + '*';

    this.isSearch = true;
    this.getElements(this.perPage,(this.page - 1) * this.perPage);

  }

  /*
   * reset the search fields
   */
  reset(): void
  {
    this.isSearch = false;

    this.accessionNumber = '';
    this.patientName = '';
    this.patientBirthDate = '';
    this.patientID = '';
    this.patientSex =  '';
    this.studyDate = '';
    this.studyDescription = '';

    this.getElements(this.perPage,(this.page - 1) * this.perPage);
  }

  getTagsString(patient: Patient) : string
  {
    let final = '';
    for (let tag of Object.keys(patient.MainDicomTags)) {
      if (tag != 'PatientName' && tag != 'PatientID' && tag != 'PatientBirthDate'
          && tag != 'PatientSex') {
          final = final + tag + ': ' + patient.MainDicomTags[tag] + '<br>';
      }
    }
    if (final == '') {
      final = 'No other DICOM tags';
    }
    return final;
  }
}
