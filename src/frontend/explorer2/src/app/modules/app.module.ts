import {BrowserModule} from '@angular/platform-browser';
import {FormsModule} from '@angular/forms';
import {HttpClientModule, HttpClient} from '@angular/common/http';
import {HttpModule} from '@angular/http';
import {NgModule} from '@angular/core';
import {TranslateModule, TranslateLoader} from '@ngx-translate/core';
import {TranslateHttpLoader} from '@ngx-translate/http-loader';

import { FileDropModule } from 'ngx-file-drop/lib/ngx-drop';

import {AppComponent} from '../components/app/app.component';
import {AppRoutingModule} from './app-routing.module';
import {ConfigComponent} from '../components/config/config.component';
import {ConnectComponent} from '../components/connect/connect.component';
import {HomeComponent} from '../components/home/home.component';
import {InstancesListComponent} from '../components/lists/instances-list.component';
import {NavBar} from '../components/nav-bar/nav-bar.component';
import {PatientsListComponent} from '../components/lists/patients-list.component';
import {PluginsComponent} from '../components/plugins/plugins.component';
import {QueryComponent} from '../components/query-retrieve/query.component';
import {RetrieveComponent} from '../components/query-retrieve/retrieve.component';
import {RESTService} from '../services/rest.service';
import {SeriesListComponent} from '../components/lists/series-list.component';
import {StudiesListComponent} from '../components/lists/studies-list.component';
import {UploadComponent} from '../components/upload/upload.component';
import {AnonymizationComponent} from "../components/lists/subComponents/anonymization.component";
import {PermissionsService} from "../services/permissions.service";
import {DeleteComponent} from "../components/lists/subComponents/delete.component";
import {TagsComponent} from "../components/lists/subComponents/tags.component";
import {TagsModalComponent} from "../components/lists/subComponents/tags-modal.component";
import {PreviewComponent} from "../components/lists/subComponents/preview.component";
import {Redirect} from "../redirect";
import {SendComponent} from "../components/lists/subComponents/send.component";
import {BrowserAnimationsModule} from "@angular/platform-browser/animations";
import {ToastModule, ToastsManager} from "ng2-toastr";
import {SetupComponent} from "../components/config/setup.component";

export function HttpLoaderFactory(http: HttpClient) {
  return new TranslateHttpLoader(http, './assets/i18n/', '.json');
}

@NgModule({
  declarations: [
    AppComponent,
    ConfigComponent,
    ConnectComponent,
    HomeComponent,
    InstancesListComponent,
    NavBar,
    PatientsListComponent,
    PluginsComponent,
    QueryComponent,
    Redirect,
    RetrieveComponent,
    SendComponent,
    SeriesListComponent,
    SetupComponent,
    StudiesListComponent,
    TagsComponent,
    UploadComponent,
    AnonymizationComponent,
    DeleteComponent,
    TagsModalComponent,
    PreviewComponent
  ],
  imports: [
    AppRoutingModule,
    BrowserModule,
    BrowserAnimationsModule,
    FormsModule,
    HttpClientModule,
    HttpModule,
    FileDropModule,
    ToastModule.forRoot(),
    TranslateModule.forRoot({
      loader: {
        provide: TranslateLoader,
        useFactory: HttpLoaderFactory,
        deps: [HttpClient]
      }
    })
  ],
  providers: [
    RESTService,
    PermissionsService],
  bootstrap: [AppComponent]
})
export class AppModule { }
