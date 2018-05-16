import { Injectable }       from '@angular/core';
import {
  CanActivate, Router,
  ActivatedRouteSnapshot,
  RouterStateSnapshot
}                           from '@angular/router';
import {PermissionsService} from "../services/permissions.service";


@Injectable()
export class ConnectedGuard implements CanActivate {
  constructor( private router: Router, private permissions: PermissionsService) {}

  canActivate(route: ActivatedRouteSnapshot, state: RouterStateSnapshot): boolean {

    let url: string = state.url;

    return this.checkRights(url);
  }

  checkRights(url: string): boolean
  {

    if(this.permissions.isConnected == true)
    {
        return true;
    }

    else
    {
      this.permissions.readPermissions().then(() =>
      {
        if(this.permissions.isConnected == false)
        {
          this.router.navigate(['/connect']);
        }
        else
        {
            console.log('Access granted');
            this.router.navigate([url]);
        }
      }, ()=> this.router.navigate(['/connect']));
      this.router.navigate(['/redirect']);
      return false;
    }
  }
}
