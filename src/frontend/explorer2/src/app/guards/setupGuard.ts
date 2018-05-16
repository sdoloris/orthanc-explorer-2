import { Injectable }       from '@angular/core';
import {
  CanActivate, Router,
  ActivatedRouteSnapshot,
  RouterStateSnapshot
}                           from '@angular/router';
import {PermissionsService} from "../services/permissions.service";
import {RESTService} from "../services/rest.service";

@Injectable()
export class SetupGuard implements CanActivate {
  constructor(private service: RESTService, private router: Router, private permissions: PermissionsService) {}

  canActivate(route: ActivatedRouteSnapshot, state: RouterStateSnapshot): boolean {

    let url: string = state.url;
    console.log("Calling setup guard " + url);

    return this.checkRights(url);
  }

  checkRights(url: string): boolean
  {

    this.service.existsDatabase().then(res=>
    {
      if(res.json().isInitialisated == false)
        this.router.navigate(['/setup']);
      else
        this.router.navigate(['/connect']);
    },()=>this.router.navigate(['/connect']));


    return false;

  }
}
