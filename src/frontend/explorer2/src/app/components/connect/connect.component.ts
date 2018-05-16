//Connection/Login page
import {Component, OnInit} from '@angular/core';
import {PermissionsService} from "../../services/permissions.service";
import {Router} from "@angular/router";
import {RESTService} from "../../services/rest.service";
import {AppComponent} from "../app/app.component";

@Component({
	selector: 'connect',
	templateUrl: './connect.component.html'
})

export class ConnectComponent implements OnInit{


  username: string;
	password: string;

	constructor(private service: RESTService, private permissions: PermissionsService, private router: Router, private app: AppComponent){};

  ngOnInit(): void
  {
    this.service.existsDatabase().then(res=> {
      if (res.json().isInitialisated == false) {
        this.router.navigate(['/setup']);
      }
      else
      {
        this.permissions.readPermissions().then(()=>{
          if(this.permissions.isConnected == true)
            this.router.navigate(['/home']);
        });
      }
    },()=>{});


  }

  /*
   * Logs the user in
   */
	login(): void {
    this.service.connection(this.username, this.password).then(res => {
      if (res.status == 200) {
        this.permissions.readPermissions();
        this.router.navigate(['/home']);
      }
      else {
        this.app.toastError('login', 'connection');
        console.log("Wrong password");
      }
    }, ()=> {this.app.toastError('noResponse', 'connection')});
  }
}


