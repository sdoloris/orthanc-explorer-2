//Configuration page
import {Component} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {Modality, User} from '../../services/miscellaneous.classes';
import {RESTService} from '../../services/rest.service';
import {TranslateService} from '@ngx-translate/core';
import {AppComponent} from "../app/app.component";
import {PermissionsService} from "../../services/permissions.service";

@Component({
  selector: 'config',
  templateUrl: './config.component.html'
})

export class ConfigComponent {
  loadingModalities = false;
  loadingPlugins = false;
  modalities: Modality[];
  private newPermissions: string[];
  private permissions = ['Read', 'Write', 'Delete', 'SuperUser'];
  plugins: string[];
  tags: string[];
  users: User[];

  constructor(private permission: PermissionsService, http: HttpClient, private restService: RESTService, private translateService: TranslateService, private app: AppComponent) {
    this.modalities = [];
    this.getModalities();
    this.plugins = [];
    this.getPlugins();
    this.tags = [];
    this.getTags();
    this.users = [];
    this.getUsers();
    this.newPermissions = [];
  }

  /*
  Adds a modality to the Orthanc config file.
  PARAMETERS:
    -name: name given to the remote modality
    -aet: AET of the remote modality
    -ip: IP address of the remote modality
    -port: Number of the port to reach the remote modality
    -patch: patch field (usually set to "generic"
   */
  addModality(name: string, aet: string, ip: string, port: number, patch: string): void {
    let modality = new Modality(name, aet, ip, port, patch);
    if (name != "" && aet != "" && ip != "" && port < 65536)
      this.restService.addModality(modality)
        .then(res => {
          if (res.status == 200) {
            this.modalities.push(modality);
            this.showRestartConfirmation();
          }
          else {
          console.error("Unable to add the modality " + modality + "\nError status: " + res.status);
          this.app.toastJobResult(false, 'Add modality');
        }
      });
  }

  /*
  Adds a plugin path to the Orthanc config file
  PARAMETERS:
    -path: path to the file or the directory where the plugin is located
   */
  addPlugin(path: string): void {
    if (path != undefined) {
      this.restService.addPlugin(path)
        .then(res => {
          if(res.status == 200) {
            this.showRestartConfirmation();
            this.plugins.push(path);
          }
        });
    }
  }

  /*
  Adds a tag to the list of possible tags in the database.
  PARAMETERS:
    -tag: the new tag to be added
   */
  addTag(tag: string): void {
    if (tag != "")
      this.restService.addTag(tag)
        .then(res => {
          if (res.status == 200) {
            this.tags.push(tag);
          }
          else {
            console.error("Unable to add the tag " + tag +
              "\nError status: " + res.status + "\n" + res.statusText);
          }
        })
  }

  /*
  Add a user to the user database and the list of users displayed.
  PARAMETERS:
    -name: Name given to the user
    -password: Password assigned to the user
   */
  addUser(name: string, password: string): void {
    let permissions = Object.assign([], this.newPermissions);
    if (name != "" && password != "") {
      this.restService.addUser(name, password)
        .then(res => {
          if (res.status == 200) {
            if(res.json().errorCode != 0)
            {
              this.app.toastUserError(res.json().errorCode);
            }
            else
            {
            let userPermissions = [];
            for (let permission of permissions) {
              this.restService.addPermission(name, permission)
                .then(added => {
                  if (added.status != 200) {
                    console.error("Unable to add the permission " + permission + "to the user " + name +
                      "\nError status: " + added.status);
                    this.users.push(new User(name, userPermissions))
                    return;
                  } else {
                    userPermissions.push(permission);
                  }
                })
            }
              this.users.push(new User(name, permissions));
            }
          }
          else {
            console.error("Unable to add the user.\nError status: " + res.status + "\n" + res.statusText);
            return;
          }
        });
    }
  }

  /*
  Applies the changes of permissions done in frontend to backend
  PARAMETERS:
    -user: the user for which you want to change the permissions
   */
  applyPermissionsChanges(user: User): void {
    let oldPermissions: string[];
    this.restService.postUserPermissions(user.name)
      .then(res => {
        oldPermissions = res.json().permissions;
        for (let perm of user.permissions) {
          if (oldPermissions.indexOf(perm) < 0) {
            this.restService.addPermission(user.name, perm)
              .then(add => {
                if (add.status != 200) {
                  console.error("Unable to add the permission: " + perm + "\nError status: " + add.status);
                  user.permissions = oldPermissions;
                  return;
                }
                else {
                  oldPermissions.push(perm);
                }
              })
          }
        }
        for (let perm of oldPermissions) {
          if (user.permissions.indexOf(perm) < 0) {
            this.restService.removePermission(user.name, perm)
              .then(remove => {
                if (remove.status != 200) {
                  console.error("Unable to remove the permission: " + perm +
                    "\nError status: " + remove.status + "\n" + remove.statusText);
                  user.permissions = oldPermissions;
                  return;
                }
              })
          }
        }
      });
  }


  /*
  Changes the permissions that will be assigned to a new user (newPermissions).
  (Called when a checkbox changes its state.)
  PARAMETERS:
    -perm: permission that will be added or removed from the newPermissions list
   */
  changeNewPermission(perm: string): void {
    let newIndex = this.newPermissions.indexOf(perm);
    if (newIndex >= 0) {
      this.newPermissions.splice(newIndex, 1);
    } else {
      this.newPermissions.push(perm);
    }
  }

  /*
  Changes the permissions associated to a user
  (Called when a checkbox changes its state.)
  PARAMETERS:
    -user: the user for which we want to change the permissions
    -perm: permission that will be added or removed from the user
   */
  changePermission(user: User, perm: string): void {
    let userIndex = user.permissions.indexOf(perm);
    if (userIndex >= 0) {
      this.restService.removePermission(user.name, perm)
        .then(removed => {
          if (removed.status != 200) {
            console.error("Unable to remove the permission: " + perm +
              "\nError status: " + removed.status + "\n" + removed.statusText);
            return;
          }
          else {
            user.permissions.splice(userIndex, 1);
            if(user.name == this.permission.getUserName())
            {
              this.permission.readPermissions();
            }
          }
        });
    }
    else {
      this.restService.addPermission(user.name, perm)
        .then(added => {
            if (added.status != 200) {
              console.error("Unable to add the permission: " + perm + "\nError status: " + added.status);
              return;
            }
            else {
              user.permissions.push(perm);
              if(user.name == this.permission.getUserName())
              {
                this.permission.readPermissions();
              }
            }
          }
        );
    }
  }

  /*
  Deletes a modality from the config file.
  PARAMETERS:
    -modality: the modality to be removed
   */
  deleteModality(modality: Modality): void {
    let message: string;
    this.translateService.get('CONFIG.DELETE_MODALITY').subscribe(
      value => {
        message = value;
      }
    );
    if(confirm(message)) {
      this.restService.removeModality(modality.name).then(()=>{
        this.showRestartConfirmation();
      },()=> this.app.toastJobResult(false, 'Delete modality'));
      this.modalities.splice(this.modalities.indexOf(modality), 1);
    }
  }

  /*
  Deletes a plugin's path from the config file.
  PARAMETERS:
    -plugin: the plugin to be removed
   */
  deletePlugin(path: string): void {
    let message: string;
    this.translateService.get('CONFIG.DELETE_PLUGIN').subscribe(
      value => {
        message = value;
      }
    );
    if(path != undefined && confirm(message)) {
      this.restService.removePlugin(path)
        .then(res => {
          if(res.status == 200) {
            this.plugins.splice(this.plugins.indexOf(path), 1);
            this.showRestartConfirmation();
          }
        })
    }
  }

  /*
  Deletes a tag of the list of possible tags in the database.
  PARAMETERS:
    -tag: tag to be removed
   */
  deleteTag(tag: string): void {
    let message: string;
    this.translateService.get('CONFIG.DELETE_TAG').subscribe(
      value => {
        message = value;
      }
    );
    if (confirm(message)) {
      this.restService.removeTag(tag)
        .then(removed => {
          if (removed.status == 200) {
            let index = this.tags.indexOf(tag);
            this.tags.splice(index, 1);
          }
          else {
            console.error("Unable to delete the tag " + tag +
              "\nError status: " + removed.status + "\n" + removed.statusText);
          }
        })
    }
  }

  /*
  Deletes a user from the user database.
  PARAMETERS:
    -user: the user to be removed
   */
  deleteUser(user: User): void {
    let message: string;
    this.translateService.get('CONFIG.DELETE_USER').subscribe(
      value => {
        message = value;
      }
    );
    if (confirm(message)) {
      this.restService.removeUser(user.name)
        .then(removed => {
          if (removed.status == 200) {
            if(removed.json().errorCode != 0)
            {
              this.app.toastUserError(removed.json().errorCode);
            }
            else
            {
              this.users.splice(this.users.indexOf(user), 1);
            }
          }
          else {
            console.error("Unable to delete the user " + user.name +
              "\nError status: " + removed.status + "\n" + removed.statusText);
          }
        })
    }
  }

  /*
  Gets the list of different modalities known to Orthanc
  and stores them in the modalities list of the component.
   */
  getModalities(): void {
    this.loadingModalities = true;
    this.modalities = [];
    this.restService.getModalities()
      .then(res => {
        if (res.json() != null) {
          this.modalities = res.json()
            .map(modality => {
              return new Modality(modality.name, modality.aet, modality.ip, modality.port, modality.patch);
            })
        }
      });
    this.loadingModalities = false;
  }

  /*
  Gets the list of paths of the different plugins added to Orthanc
  and stores them in the plugins list of the component.
   */
  getPlugins(): void {
    this.loadingPlugins = true;
    this.plugins = [];
    this.restService.getPluginPaths()
      .then(res => {
        this.plugins = res.json().paths;
      });
    this.loadingPlugins = false;
  }

  /*
  Gets the list of different tags that can be used on instances
  and stores them in the tags list of the component.
   */
  getTags(): void {
    this.restService.getTagsList()
      .then(res => {
        if (res.json() != null)
          this.tags = res.json().tags;
        else
          this.tags = [];
      });
  }

  /*
  Gets the list of different users of the Orthanc Explorer
  and stores them in the users list of the component.
   */
  getUsers(): void {
    let userNames: string[];
    this.users = [];
    this.restService.getUsernameList()
      .then(res => {
        if (res.json().userList != null) {
          userNames = res.json().userList;
          for (let name of userNames) {
            this.restService.postUserPermissions(name)
              .then(res => {
                let permissions;
                if (res.json() != null)
                  permissions = res.json().permissions;
                else
                  permissions = [];
                let user = new User(name, Object.assign([], permissions));
                this.users.push(user);
              });
          }
        }
      });
  }

  /*
  Checks if a user has a certain permission or not.
  PARAMETERS:
    -user: the user on which we want to check if he has the permission
    -perm: the permission to check
  RETURN:
    Boolean indicating if the user has the permission or not.
   */
  hasPermission(user: User, perm: string): boolean {
    return user.permissions.indexOf(perm) >= 0
  }

  /*
  Modifies the different fields of a modality.
  PARAMETERS:
    -modality: modality to be modified
    -nAET: new AET for the modality
    -nIP: new IP for the modality
    -nPort: new port for the modality
    -nPatch: new patch for the modality
   */
  modifyModality(modality: Modality, nAET: string, nIP: string, nPort: number, nPatch: string): void {
    this.restService.modifyModality(modality.name, nAET, nIP, nPort, nPatch)
      .then(res => {
          this.modalities[this.modalities.indexOf(modality)] = new Modality(modality.name, nAET, nIP, nPort, nPatch);
          this.showRestartConfirmation();
      });
    document.getElementById('modify ' + modality.name).style.display = "none";
  }

  /*
  Modifies the string associated to a tag
  PARAMETERS:
    -tag: current tag string
    -nTag: new tag string
    -tagNumber: index of the tag in the tags list
   */
  modifyTag(tag: string, nTag: string, tagNumber: number): void {
    this.restService.modifyTag(tag, nTag)
      .then(modified => {
        if (modified.status == 200) {
          this.tags[this.tags.indexOf(tag)] = nTag;
        }
        else {
          console.error("Unable to modify the tag " + tag +
            "\nError status: " + modified.status + "\n" + modified.statusText);
        }
      });
    document.getElementById('tag' + tagNumber).style.display = "none";
  }

  /*
  Resets the password of the user.
  The password will be identical to its username after reset.
  PARAMETERS:
    -user: the user whose password should be reset
   */
  resetPassword(user: User): void {
    let message: string;
    this.translateService.get('CONFIG.CONFIRM_RESET').subscribe(
      value => {
        message = value;
      }
    );
    if (confirm(message)) {
      this.restService.reinitialiseUser(user.name)
        .then(res => {
          if (res.status == 200) {
            console.log("The passwort of the user is now equal to its username.");
          }
          else {
            console.error("Impossible to reinitialise the user.");
          }
        });
    }
  }

  /*
  Makes the division showing the details of a modality visible.
  PARAMETERS:
   -modalityName: name of the modality (functions as an id)
   */
  showDetailsModality(modalityName: string): void {
    document.getElementById('details ' + modalityName).style.display = "block";
  }

  /*
  Makes the division used to modify a modality visible.
  PARAMETERS:
   -modalityName: name of the modality (functions as an id)
   */
  showModifyModality(modalityName: string): void {
    document.getElementById('modify ' + modalityName).style.display = "block";
  }

  /*
  Makes the division used to modify a tag visible.
  PARAMETERS:
    -tagNumber: index of the tag in the tags list (functions as an id)
   */
  showModifyTag(tagNumber: number): void {
    document.getElementById('tag' + tagNumber).style.display = "block";
  }


  /*
  Shows a confirmation message to restart the Orthanc core.
   */
  private showRestartConfirmation(): void {
    let message: string;
    this.translateService.get('CONFIG.RESTART').subscribe(
      value => {
        message = value;
      }
    );
    if(confirm(message)) {
      this.restService.restart()
        .then(res => {
          if(res.status != 200) {
            console.error("Impossible to restart the Orthanc core\n" +
            "Error status: " + res.status + "\n" + res.statusText);
          }
        })
    }
  }
}
