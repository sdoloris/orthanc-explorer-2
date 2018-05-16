import {Component, Input, OnInit} from '@angular/core';
import {ActivatedRoute, Router} from "@angular/router";
import { Location }                 from '@angular/common';
import "rxjs/add/operator/switchMap";

import {ListComponent} from './list.component';
import {Statistics, Study} from "../../services/miscellaneous.classes";
import {Patient} from "../../services/miscellaneous.classes";
import {RESTService} from "../../services/rest.service";
import {TranslateService} from "@ngx-translate/core";
import {PermissionsService} from "../../services/permissions.service";
import {AppComponent} from "../app/app.component";


@Component
({
  selector: 'studies-list',
  templateUrl: 'studies-list.component.html',
  providers: [RESTService]
})

export class StudiesListComponent extends ListComponent implements OnInit
{
  @Input() relatedPatientID: string;
  relatedPatient: Patient;
  relatedPatientProtected = false;
  singlePatient = false;
  isSearch = false;
  searchIDs: Array<string>;

  studyModalities = ['CR', 'CT', 'MR', 'NM', 'PT', 'US', 'XA'];
  nbCheckedModalities = 0;
  checkedModalities = [false, false, false, false, false, false, false];

  studies: Study[] = [];
  tags = Object.keys;
  @Input() page: number;
  pageValues = new Array(1, 2, 10, 20, 50);
  totPage: number;
  perPage= 10;
  prevPerPage= 10;
  totalStudies: number;
  statistics: Statistics;

  upperLevel = 'patients';
  level= 'Study';

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

  allTags: string[];

  constructor(private service: RESTService, private router: Router, private route: ActivatedRoute, private location: Location,
              private permissions: PermissionsService, private translateService: TranslateService, private app: AppComponent){super();}

  ngOnInit(): void
  {
    this.route.params.subscribe(params => {
      if (params['id'])
      {
        this.singlePatient = true;
        this.relatedPatientID = params['id'];
        this.service.getProtected(params['id']).then(
          res => {if (res.json() == '0') {
                                          this.relatedPatientProtected = false;
                                        }
                                      else {
                                        this.relatedPatientProtected = true;
                                      }});
      }
    });

    if(this.singlePatient == false)
    {
      this.service.getStatistics()
        .then(statistics => {
          this.statistics = statistics.json();
          this.totalStudies = this.statistics.CountStudies;
          this.totPage = Math.ceil(this.totalStudies / this.perPage);
          this.searchIDs = new Array<string>(this.perPage);
          this.getElements(this.perPage,(this.page - 1) * this.perPage);
        });
      this.page = 1;
      this.totPage = Math.ceil(this.totalStudies / this.perPage);
    }

    else
    {
      this.totalStudies = 0;
      this.service.getPatientInfo(this.relatedPatientID)
        .then(res => {this.relatedPatient = new Patient(res.json())});
      this.getElements(0,0);
    }

    this.service.getTagsList()
      .then(res => {
        this.allTags = [];
        if(res.json() != null)
          this.allTags = res.json().tags;
      });
  }

  /*
   * Retrieve 'nb' studies from the server, starting from the 'from'th study
   */
  getElements(nb: number, from: number): void
  {
    if(this.singlePatient)
    {
      this.service.getChildren('patients', this.relatedPatientID)
        .then(res =>
        {
          switch (res.status)
          {
            case 403:
              this.permissions.errorRedirect();
              break;
            default:
              this.studies = res.json().map(study => {
                return new Study(study);
              });
              this.retrieveTags();
              if(this.studies.length <= 0)
              {
                this.app.toastInfo('noResult');
              }
          }

        });
    }
    else
    {
      if(!this.isSearch)
      {
        this.service.getStudies(nb, from)
          .then(res =>
          {
            switch (res.status)
            {
              case 403:
                this.permissions.errorRedirect();
                break;
              default:
                this.studies = res.json().map(study => {
                  return new Study(study);
                });
                this.retrieveTags();
                if(this.studies.length <= 0)
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
              this.studies = new Array<Study>(this.searchIDs.length);
              if(this.studies.length <= 0)
              {
                this.app.toastInfo('noResult');
              }
              for(let i = 0, j = 0; i < this.studies.length; i++, j++)
              {
                this.service.getStudyInfo(this.searchIDs[i])
                  .then(res => {
                    this.studies[i] = new Study(res.json());
                    this.service.getTags(this.studies[i].ID)
                      .then(res => {
                        this.studies[i].tags = [];
                        if(res.json() != null)
                          this.studies[i].tags = res.json().tags;
                      });
                  });
              }
          }
        })
      }
    }
  }

  /*
   * Retrieve the tags of the studies
   */
  retrieveTags()
  {
    for(let i = 0; i < this.studies.length; i++)
    {
      this.service.getTags(this.studies[i].ID)
        .then(res => {
          this.studies[i].tags = [];
          if(res.json() != null)
            this.studies[i].tags = res.json().tags;
        });
    }
  }

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
   * Load a given page
   */
  requestPage(): void
  {
    this.borderPage();
    this.getElements(this.perPage, (this.page-1) * this.perPage);
  }

  /*
   * Navigates to the selected study
   */
  onSelect(event: any, serieID: string): void
  {
    if (event.target.nodeName == "TD") {
      this.router.navigate(['/series', {id: serieID}]);
    }
  }

  /*
   * Download the corresponding zip file
   */
  downloadZip(level: string, ID: string, type: string)
  {
    this.service.getZIP(level, ID, type);
  }

  /*
   * Check the modality checkbox for the search
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
   * search a specific study on the server
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

  protect(value: string) {
    this.route.params.subscribe(params => {
      if (params['id'])
      {
        this.service.putProtected(params['id'],value);
        this.service.getProtected(params['id']).then(
          res => {if (res.json() == '0') {
                                          this.relatedPatientProtected = false;
                                        }
                                      else {
                                        this.relatedPatientProtected = true;
                                      }});
      }
    });
  }

  /*
  Filter the list of studies by a specific tag.
  PARAMETERS:
    -tag: tag on which the list should be filtered
   */
  filterByTag(tag: string) {
    this.studies = [];
    if(tag != '') {
      let studies: string[];
      this.service.getStudiesWithTag(tag)
        .then(res => {
          if(res.json() != null) {
            studies = res.json().studyList;
            for(let study of studies) {
              this.service.getStudyInfo(study)
                .then(studyInfo => {
                  this.studies.push(new Study(studyInfo.json()));
                })
            }
          }
        })
    }
    else {
      this.getElements(this.perPage,(this.page - 1) * this.perPage);
    }
  }

  /*
  Get the DICOM tags corresponding to a study.
  PARAMETERS:
    -study: DICOM study whose tags should be retrieved
  RETURN:
    String containing all the tags in a concatenation
   */
  getTagsString(study: Study) : string
  {
    let final = '';
    for (let tag of Object.keys(study.MainDicomTags)) {
      if (tag != 'StudyDescription' && tag != 'StudyDate' && tag != 'AccessionNumber') {
          final = final + tag + ': ' + study.MainDicomTags[tag] + '<br>';
      }
    }
    if (final == '') {
      final = 'No other DICOM tags';
    }
    return final;
  }

}
