import {Component, Input, OnInit} from '@angular/core';
import { Location } from '@angular/common';
import {ActivatedRoute, Router} from "@angular/router";

import {ListComponent} from './list.component';
import {Patient, Serie} from "../../services/miscellaneous.classes";
import {Study} from "../../services/miscellaneous.classes";
import {RESTService} from "../../services/rest.service";
import {TranslateService} from "@ngx-translate/core";
import {PluginsComponent} from "../plugins/plugins.component";



@Component
({
  selector: 'series-list',
  templateUrl: 'series-list.component.html',
  providers: [RESTService, PluginsComponent]
})

export class SeriesListComponent extends ListComponent implements OnInit
{
  relatedStudyID: string;
  relatedPatientID: string;
  relatedPatientProtected = false;

  relatedStudy: Study;
  relatedPatient: Patient;

  tags = Object.keys;

  series: Serie[];
  @Input() page: number;

  upperLevel= 'studies';


  totPage: number;
  constructor(private service: RESTService, private route: ActivatedRoute, private router: Router, private location: Location,
              private translateService: TranslateService, private plugins: PluginsComponent){super();}

  /*
  Method called on component creation
   */
  ngOnInit(): void
  {
    this.route.params.subscribe(params => {
      if (params['id'])
      {
        console.log(params);
        this.relatedStudyID = params['id'];
        this.service.getParent('studies', this.relatedStudyID, 'patient')
          .then(res => {
            this.relatedPatient = new Patient(res.json());
            this.service.getProtected(this.relatedPatient.ID).then(
              res => {if (res.json() == '0') {
                                              this.relatedPatientProtected = false;
                                            }
                                          else {
                                            this.relatedPatientProtected = true;
                                          }});
          });
          this.service.getStudyInfo(this.relatedStudyID)
            .then(res => this.relatedStudy = new Study(res.json()));
      }
    });
    this.getElements();
  }

  /*
   * Retrieve 'nb' series from the server, starting from the 'from'th serie
   */
  getElements(): void
  {
    this.service.getChildren('studies', this.relatedStudyID)
      .then(res => {
        this.series = res.json().map(serie => {
          return new Serie(serie);
        })
      })
  }

  /*
   * Navigates to the selected serie
   */
  onSelect(serieID: string): void
  {
    this.router.navigate(['/instances', {id: serieID}]);
  }

  /*
   * Download the corresponding zip file
   */
  downloadZip(level: string, ID: string, type: string)
  {
    this.service.getZIP(level, ID, type);
  }

  /*
  Protect or unprotect the patient corresponding to the series
  PARAMETERS:
    -value: string indicating if the patient should be protected (1) or not (0)
   */
  protect(value: string) {
    this.route.params.subscribe(params => {
      if (this.relatedPatient)
      {
        this.service.putProtected(this.relatedPatient.ID,value);
        this.service.getProtected(this.relatedPatient.ID).then(
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
  Get the DICOM tags corresponding to a series in a single string.
  PARAMETERS:
    -serie: DICOM series whose tags you want to retrieve
  RETURN:
    String containing all the different tags in a concatenation
   */
  getTagsString(serie: Serie) : string
  {
    let final = '';
    for (let tag of Object.keys(serie.MainDicomTags)) {
      if (tag != 'SeriesDescription' && tag != 'SeriesDate'
          && tag != 'Modality' && tag != 'SeriesNumber') {
          final = final + tag + ': ' + serie.MainDicomTags[tag] + '<br>';
      }
    }
    if (final == '') {
      final = 'No other DICOM tags';
    }
    return final;
  }
}
