//Navigation bar shown in the header of each page
import {Component} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {TranslateService} from '@ngx-translate/core';
import {PermissionsService} from "../../services/permissions.service";
import {Router} from "@angular/router";
import {RESTService} from "../../services/rest.service";

@Component({
	selector: 'navbar',
	templateUrl: 'nav-bar.component.html',
	styleUrls: ['nav-bar.component.css']
})

export class NavBar{
	constructor(private service: RESTService, private router: Router, private permissions: PermissionsService, private translate: TranslateService, http:HttpClient) {
		translate.setDefaultLang('en');
    translate.use('en');
	}

	/*
	  Function switching the language of the app
	  Parameter:
	    -language: language to which the app will switch
  */
	switchLanguage(language: string) {
		this.translate.use(language);
	}

	/*
	Disconnects a user from the plugin
	 */
	closeConnection():void
  {
    this.service.disconnect();
    this.permissions.reset();
    this.router.navigate(['/connect']);
  }

}
