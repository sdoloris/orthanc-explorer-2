import {NgModule}              from '@angular/core';
import {RouterModule, Routes}  from '@angular/router';

import {AdminGuard} from "../guards/adminGuard";
import {ConnectedGuard} from "../guards/connectedGuard";
import {ReadGuard} from "../guards/readGuard";
import {SetupGuard} from "../guards/setupGuard";
import {WriteGuard} from "../guards/writeGuard";

import {ConfigComponent} from '../components/config/config.component';
import {ConnectComponent} from '../components/connect/connect.component';
import {HomeComponent} from '../components/home/home.component';
import {InstancesListComponent} from "../components/lists/instances-list.component";
import {PatientsListComponent} from "../components/lists/patients-list.component";
import {PluginsComponent} from '../components/plugins/plugins.component';
import {QueryComponent} from "../components/query-retrieve/query.component";
import {RetrieveComponent} from "../components/query-retrieve/retrieve.component";
import {SeriesListComponent} from "../components/lists/series-list.component";
import {StudiesListComponent} from "../components/lists/studies-list.component";
import {UploadComponent} from "../components/upload/upload.component";
import {AppComponent} from "../components/app/app.component";
import {Redirect} from "../redirect";
import {SetupComponent} from "../components/config/setup.component";


//declaration of the different routes to which the app can go
const appRoutes: Routes = [
  { path: '', redirectTo: '/connect', pathMatch: 'full'},
  { path: 'config', canActivate: [AdminGuard], component: ConfigComponent },
	{ path: 'connect', component: ConnectComponent },
	{ path: 'home', canActivate: [ConnectedGuard], component: HomeComponent },
	{ path: 'instances', canActivate: [ReadGuard], component: InstancesListComponent },
	{ path: 'plugin', canActivate: [ConnectedGuard], component: PluginsComponent },
	{ path: 'patients', canActivate: [ReadGuard], component: PatientsListComponent },
  { path: 'query', canActivate: [WriteGuard], component: QueryComponent },
  { path: 'retrieve', canActivate: [WriteGuard], component: RetrieveComponent },
  { path: 'setup', component: SetupComponent },
  { path: 'series', canActivate: [ReadGuard], component: SeriesListComponent },
	{ path: 'studies', canActivate: [ReadGuard], component: StudiesListComponent },
	{ path: 'upload', canActivate: [WriteGuard], component: UploadComponent },
  { path: 'redirect', component: Redirect}
];

@NgModule({
	imports: [
		RouterModule.forRoot(appRoutes, {enableTracing:false})
	],
	exports: [
		RouterModule
	],
  providers: [
    AdminGuard,
    ConnectedGuard,
    ReadGuard,
    SetupGuard,
    WriteGuard
  ]
})

export class AppRoutingModule{}
