import { Injectable } from '@angular/core';
import {RESTService} from "./rest.service";
import {Router} from "@angular/router";
import {AppComponent} from "../components/app/app.component";

@Injectable()
export class PermissionsService {

  isConnected = false;
  readPermission= false;
  writePermission= false;
  deletePermission= false;
  adminPermission= false;

  usingDatabase = true;

  userName= 'Unregistered User';

  jsonString= '{"permissions": ["Read", "Write", "Delete", "SuperUser"], "username": "root"}';
  //jsonString: string;

  constructor(private service: RESTService, private router: Router) { }


  /*
   * Return the read permission of the user
   */
  hasReadPermission(): boolean
  {
    return this.readPermission;
  }

  /*
   * Return the write permission of the user
   */
  hasWritePermission(): boolean
  {
    return this.writePermission;
  }

  /*
   * Return the delete permission of the user
   */
  hasDeletePermission(): boolean
  {
    return this.deletePermission;
  }

  /*
   * Return the admin permission of the user
   */
  hasAdminPermission(): boolean
  {
    return this.adminPermission;
  }

  /*
   * Return the username permission of the user
   */
  getUserName(): string
  {
    return this.userName;
  }


  /*
   * Redirect the user depending of him being connected or not
   */
  errorRedirect()
  {
    this.reset();
    this.readPermissions();
    if(this.isConnected)
    {
      this.router.navigate(['/home']);
    }
    else
    {
      this.router.navigate(['/connect']);
    }
  }

  /*
   * Retrieve the permissions from the plugin
   */
  readPermissions()
  {
    return this.service.allowUsers().then(res=>
    {
      if(res.json().disableUsers)
      {
        this.isConnected = true;
        this.readPermission= true;
        this.writePermission= true;
        this.deletePermission= true;
        this.adminPermission= true;

        this.userName= 'Unregistered User';
        this.usingDatabase = false;
      }
      else
      {
        this.usingDatabase = true;
        return this.service.getPermission().then(res =>{
          let user = res.json();

          this.userName = user.username;
          let perm;
          if(user.permissions != null)
            perm = user.permissions;
          else
            perm = [];

          for (let i = 0; i < perm.length; i++) {
            if (perm[i] == "Read") {
              this.readPermission = true;
            }
            if (perm[i] == "Write") {
              this.writePermission = true;
            }
            if (perm[i] == "Delete") {
              this.deletePermission = true;
            }
            if (perm[i] == "SuperUser") {
              this.adminPermission = true;
            }

          }
          this.isConnected = true;
        });
      }
    });
  }


  /*
   * reset the permissions
   */
  reset()
  {
    this.isConnected = false;
    this.readPermission= false;
    this.writePermission= false;
    this.deletePermission= false;
    this.adminPermission= false;

    this.userName= 'Unregistered User';
  }

}
