import {Component, Input} from "@angular/core";
import {ActivatedRoute, Router} from "@angular/router";
import { Location }                 from '@angular/common';
import {TranslateService} from "@ngx-translate/core";
import {RESTService} from "../../../services/rest.service";
import {PermissionsService} from "../../../services/permissions.service";


@Component
({
  selector: 'delete',
  templateUrl: 'delete.component.html',
  providers: [RESTService]
})

export class DeleteComponent
{
  @Input() ID: string;
  @Input() level: string;

  constructor(private permissions: PermissionsService, private service: RESTService, private router: Router, private route: ActivatedRoute,
              private location: Location, private translateService: TranslateService){}


  /*
   * Delete the element
   */
  deleteElem()
  {
    console.log(this.ID);
    let message: string;
    this.translateService.get('LIST.DELETE_CONFIRM').subscribe(
      value => {
        message = value;
      }
    );
    if(confirm(message))
    {
      this.service.delete(this.ID, this.level);
      //this.location.back();
      this.router.navigate(['/patients']);
    }
  }
}
