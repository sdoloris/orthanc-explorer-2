//Main app component
import {Component, OnInit, ViewContainerRef} from '@angular/core';
import {Router} from "@angular/router";
import {PermissionsService} from "../../services/permissions.service";
import {RESTService} from "../../services/rest.service";
import {ToastsManager} from "ng2-toastr";
import {TranslateService} from "@ngx-translate/core";


declare var userName: string;

@Component({
  selector: 'app-root',
  //the navigation bar will be shown all the time in the header
  template: `<navbar></navbar><router-outlet></router-outlet>`
})


export class AppComponent implements OnInit {


  constructor(private router: Router, private permissions: PermissionsService, private service: RESTService,
              public toastr: ToastsManager, vcr: ViewContainerRef, private translateService: TranslateService)
  {
    this.toastr.setRootViewContainerRef(vcr);
  }

  ngOnInit(): void
  {

  }

  /*
   * Displays an error message
   */
  toastError(reason?: string, head?:string)
  {
    let message: string;
    let title: string;
    let key: string;
    switch(reason)
    {
      case 'login':
        key = 'TOASTR.ERROR.LOGIN';
        break;
      case'noResponse':
        key = 'TOASTR.ERROR.NORESPONSE';
        break;
      default:
        key = 'TOASTR.ERROR.DEFAULT';
        break;
    }

    this.translateService.get(key).subscribe(
      value => {
        message = value;
      });

    switch(head)
    {
      case 'connection':
        key = 'TOASTR.ERROR.CONNECTION';
        break;
      default:
        key = 'TOASTR.ERROR.TITLEDEFAULT';
        break;
    }
    this.translateService.get(key).subscribe(
      value => {
        title = value;
      });
    this.toastr.error(message, title);
  }

  /*
   * Displays an info message
   */
  toastInfo(reason?: string)
  {
    let message: string;
    let key: string;

    switch(reason)
    {
      case 'jobRegistered':
        key = 'TOASTR.INFO.JOB';
        break;
      case 'noResult':
        key = 'TOASTR.INFO.NORESULT';
        break;
      case'restartNeeded':
        key = 'TOASTR.INFO.RESTART';
        break;
      default:
        key = 'TOASTR.INFO.DEFAULT';
        break;
    }

    this.translateService.get(key).subscribe(
      value => {
        message = value;
      });

    this.toastr.info(message)
  }

  /*
   * Displays a warning message
   */
  toastWarning(reason?: string)
  {
    let key: string;
    switch(reason)
    {
      case 'accessDenied':
        key = 'TOASTR.WARNING.ACCESS';
        break;
      case 'noResult':
        key = 'TOASTR.WARNING.TIMEOUT';
        break;
      case 'timeout':
        key = 'TOASTR.WARNING.TIMEOUT';
        break;
      default:
        key = 'TOASTR.WARNING.DEFAULT';
        break;
    }

    let message: string;
    this.translateService.get(key).subscribe(
      value => {
        message = value;
      });

    this.toastr.warning(message)
  }

  /*
   * Displays a message about a job
   */
  toastJobResult(success: boolean, job: string, link?: string, id?: string)
  {
    let message: string;
    if(success)
    {
      this.translateService.get('TOASTR.JOB.SUCCESS').subscribe(
        value => {
          message = value;
        });
      message += job;

      if(link && id == undefined)
      {
        this.translateService.get('TOASTR.JOB.NAVIGATE').subscribe(
          value => {
            message += value;
          });
        this.toastr.onClickToast()
          .subscribe( toast => {
            this.router.navigate([link]);
          });
      }
      if(link && id)
      {
        this.translateService.get('TOASTR.JOB.NAVIGATE').subscribe(
          value => {
            message += value;
          });
        this.toastr.onClickToast()
          .subscribe( toast => {
            this.router.navigate([link, {id: id}]);
          });
      }
      this.toastr.success(message, '',{enableHTML: true});
    }
    else
    {
      this.translateService.get('TOASTR.JOB.FAIL').subscribe(
        value => {
          message = value;
        });
      this.toastr.error(message + job);
    }
  }

  /*
   * Displays an error at user creation/deletion
   */
  toastUserError(code: number)
  {
    let key: string;
    switch(code)
    {
      case 9:
        key = 'TOASTR.USER.NOTEXISTS_WRONGPASSWORD';
        break;
      case 11:
        key = 'TOASTR.USER.ALREADYEXISTS';
        break;
      case 14:
        key = 'TOASTR.USER.LASTSUPERUSER';
        break;
      default:
        key = 'TOASTR.ERROR.DEFAULT';
    }
    let message: string
    this.translateService.get(key).subscribe(
      value => {
        message = value;
      });
    this.toastr.error(message);
  }

  /*
   * Displays an error at setup
   */
  toastSetupError(code :number)
  {
    let key: string;
    switch(code)
    {
      case 1:
        key = 'TOASTR.SETUP.SQLERROR';
        break;
      case 2:
        key = 'TOASTR.SETUP.PATHERROR';
        break;
      case 4:
        key = 'TOASTR.SETUP.USERERROR';
        break;
      case 5:
        key = 'TOASTR.SETUP.USERERROR';
        break;
      case 6:
        key = 'TOASTR.SETUP.SETERROR';
        break;
      default:
        key = 'TOASTR.ERROR.DEFAULT';
    }
    let message: string
    this.translateService.get(key).subscribe(
      value => {
        message = value;
      });
    this.toastr.error(message);
  }
}
