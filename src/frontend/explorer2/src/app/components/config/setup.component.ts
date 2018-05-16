import {Component, Input} from "@angular/core";
import {User} from "../../services/miscellaneous.classes";
import {RESTService} from "../../services/rest.service";
import {TranslateService} from "@ngx-translate/core";
import {AppComponent} from "../app/app.component";
import {PermissionsService} from "../../services/permissions.service";
import {Router} from "@angular/router";

@Component({
  selector: 'setup',
  templateUrl: './setup.component.html'
})

export class SetupComponent
{


  usingDatabase = true;
  @Input() name: string;
  @Input() password: string;
  @Input() passwordBis: string;
  @Input() path: string;
  constructor(private router: Router, private restService: RESTService, private translateService: TranslateService, private permissions: PermissionsService, private app: AppComponent){}


  /*
   * Switch the usingDatabase variable between true and false
   */
  changeUse()
  {
    this.usingDatabase = !this.usingDatabase;
  }

  /*
   * asks the plugin to create the database and wait the response to proceed further
   */
  confirm()
  {
    console.log("Use of DB: "+this.usingDatabase+". Name: "+this.name+" and password: "+this.password);
    this.restService.initDB(this.path, !this.usingDatabase, this.name, this.password).then(res=>
    {
      if(res.json().errorCode == 0)
      {
        if(this.usingDatabase)
        {
          this.restService.connection(this.name, this.password).then(res => {
            if (res.status == 200) {
              this.permissions.readPermissions();
              this.router.navigate(['/home']);
            }
          });
        }
        else
        {
          this.permissions.readPermissions();
          this.router.navigate(['/home']);
        }
      }
      else
      {
        this.app.toastSetupError(res.json().errorCode);
      }
    },()=>{});
  }
}
