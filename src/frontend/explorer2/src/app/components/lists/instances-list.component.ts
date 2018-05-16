import {Component, Input, OnInit} from '@angular/core';
import {ActivatedRoute, Router} from "@angular/router";
import { Location }                 from '@angular/common';

import {ListComponent} from './list.component';
import {Instance, Patient, Study} from "../../services/miscellaneous.classes";
import {Serie} from "../../services/miscellaneous.classes";
import {RESTService} from "../../services/rest.service";
import {TranslateService} from "@ngx-translate/core";
import {PluginsComponent} from "../plugins/plugins.component";


@Component
({
  selector: 'instances-list',
  templateUrl: 'instances-list.component.html',
  providers: [RESTService, PluginsComponent]
})

export class InstancesListComponent extends ListComponent implements OnInit
{
  relatedSerieID: string;
  relatedPatient: Patient;
  relatedPatientProtected = false;
  relatedStudy: Study;
  relatedSerie: Serie;

  preview: string;

  instances: Instance[];
  tags = Object.keys;
  @Input() page: number;

  upperLevel= 'series';

  constructor(private service: RESTService, private router: Router, private route: ActivatedRoute, private location: Location,
              private translateService: TranslateService, private plugins: PluginsComponent){super();}

  ngOnInit(): void
  {
    this.route.params.subscribe(params => {
      if (params['id']) {
        this.relatedSerieID = params['id'];
        this.service.getParent('series', this.relatedSerieID, 'patient')
          .then(res =>
          {
            this.relatedPatient = new Patient(res.json());
            this.service.getProtected(this.relatedPatient.ID).then(
              res => {if (res.json() == '0') {
                                              this.relatedPatientProtected = false;
                                            }
                                          else {
                                            this.relatedPatientProtected = true;
                                          }});
          });
        this.service.getParent('series', this.relatedSerieID, 'study')
          .then(res => {
            console.log(res);
            this.relatedStudy = new Study(res.json());
          });
        this.service.getSerieInfo(this.relatedSerieID)
          .then(res => this.relatedSerie = new Serie(res.json()));
      }
    });
    this.getElements();
  }

  /*
   * Retrieve 'nb' instances from the server, starting from the 'from'th instance
   */
  getElements(): void
  {
    this.service.getChildren('series', this.relatedSerieID)
      .then(res => {
        this.instances = res.json().map(instance => {
          return new Instance(instance);
        })
      });
  }

  /*
   * Download the corresponding zip file
   */
  downloadZip(level: string, ID: string, type: string)
  {
    this.service.getZIP(level, ID, type);
  }

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
  Get the DICOM tags in form of a string.
  PARAMETERS:
    -instance: DICOM instance from which you want to fetch the tags
  RETURN:
    the different tags concatenated in a single string
   */
  getTagsString(instance: Instance) : string
  {
    let final = '';
    for (let tag of Object.keys(instance.MainDicomTags)) {
      if (tag != 'InstanceNumber' && tag != 'InstanceCreationDate') {
          final = final + tag + ': ' + instance.MainDicomTags[tag] + '<br>';
      }
    }
    if (final == '') {
      final = 'No other DICOM tags';
    }
    return final;
  }
}
