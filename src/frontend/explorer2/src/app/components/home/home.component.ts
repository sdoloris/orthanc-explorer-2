//Home/Welcome page
import {Component} from '@angular/core';
import {PermissionsService} from "../../services/permissions.service";
import {RESTService} from "../../services/rest.service";
import {AppComponent} from "../app/app.component";


@Component({
	selector: 'home',
	templateUrl: './home.component.html'
})

export class HomeComponent {

  constructor(private permissions: PermissionsService, private restService: RESTService, private app: AppComponent){}

  /*
  Changes the password of the user to the one specified in new1 and new2
  provided they are the same and current corresponds to the current password
  PARAMETERS:
    -current: current user password
    -new1: new password
    -new2: repetition of the new password
   */
  changePassword(current: string, new1: string, new2: string): boolean {
    if(new1 != new2) {
      console.error("The two new passwords provided are different.");
      return false;
    }
    //the check if current corresponds to the current password is done in backend
    this.restService.changePassword(this.permissions.getUserName(), current, new2)
      .then(res => {
        if(res.status == 200) {
          if(res.json().errorCode == 9) {
            this.app.toastUserError(res.json().errorCode);
            return false;
          }
          if(res.json().errorCode == 0) {
            this.hideChangePassword();
            return true;
          }
        }
      });
    return false;
  }

  /*
  Makes the division that permits to change the password visible
   */
  showChangePassword() {
    document.getElementById('changePassword').style.display = "block";
    document.getElementById('passwordButton').style.display = "none"
  }

  /*
  Hides the division that permits to change the password
   */
  hideChangePassword()
  {
    document.getElementById('changePassword').style.display = "none";
    document.getElementById('passwordButton').style.display = "block"
  }
}
