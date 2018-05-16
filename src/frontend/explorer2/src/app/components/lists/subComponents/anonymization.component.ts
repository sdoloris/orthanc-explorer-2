import {Component, Input} from "@angular/core";
import {ActivatedRoute, Router} from "@angular/router";
import {TranslateService} from "@ngx-translate/core";
import {RESTService} from "../../../services/rest.service";
import {PermissionsService} from "../../../services/permissions.service";
import {AppComponent} from "../../app/app.component";


@Component
({
  selector: 'anonymize',
  templateUrl: 'anonymization.component.html',
  providers: [RESTService]
})

export class AnonymizationComponent
{
  @Input() anonymizedName: string;
  showAnonymizeBox = false;

  @Input() ID: string;
  @Input() level: string;

  constructor(private service: RESTService, private router: Router, private route: ActivatedRoute, private permissions: PermissionsService,
              private translateService: TranslateService, private app: AppComponent){}



  /*
   * Show the box to anonymize the element
   */
  anonymizeBox():void
  {
    this.showAnonymizeBox = true;
  }

  /*
   * Anonymize the element
   */
  anonymize(): void
  {

    this.service.anonymize(this.level, this.ID, this.anonymizedName).then(()=>
    {
      this.app.toastJobResult(true,'Anonymize');
      //this.router.navigate(['/patients'])
    },()=> this.app.toastJobResult(false, 'Anonymize'));

    this.app.toastInfo('jobRegistered');
  }
}
