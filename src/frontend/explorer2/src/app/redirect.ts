import {Component} from "@angular/core";
import {PermissionsService} from "./services/permissions.service";
import {Router} from "@angular/router";

@Component({
  selector: 'connect',
  template: `
    <p>You will be redirected shortly</p>
    <a [routerLink]="['connect']">If you are not redirected after 30 seconds, click here</a>
  `
})

export class Redirect
{

}
