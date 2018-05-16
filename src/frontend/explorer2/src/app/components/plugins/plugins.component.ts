import {NgModule, Component} from '@angular/core';
import {BrowserModule} from '@angular/platform-browser';
import {HttpModule} from '@angular/http';
import 'rxjs/Rx';
import {RESTService} from '../../services/rest.service';
import {Plugin} from '../../services/miscellaneous.classes';


@Component({
  selector: 'plugins',
  templateUrl: './plugins.component.html'
})

export class PluginsComponent {
  private loading = false;
  private RestService: RESTService;
  private plugins: Plugin[];
  constructor(private Restservice: RESTService) {
    this.RestService = this.Restservice;
    this.plugins = [];
    this.doSearch();
  }

  /*
  Do a search for all the different activated plugins
   */
	doSearch() {
		this.loading = true;
		this.RestService.getPluginsName().then(res => {
		  let pluginNames = res.json()
        .map(name => {
          return name;
        });
			for (let name of pluginNames) {
				if (name != 'explorer.js') {
					this.loading = true;
					this.RestService.getPluginInfo(name).then(res => {
					  this.plugins.push(new Plugin(res.json(), 'Plugin'));
					});
				}
			}
			this.loading = false;
			});
	}

	/*
	Verifies if a plugin is activated
	PARAMETERS:
	  -pluginName: name of the plugin to be verified
	RETURN:
	  Boolean indicating whether or not the plugin is activated
	 */
  isPluginActivated(pluginName : string) {
    for (let plugin of this.plugins) {
      if (plugin.ID == pluginName) {
        return true;
      }
    }
    return false;
  }
}
