import { Component, Input, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { NotifierService } from 'angular-notifier';
import { PumpSettingsService } from '../pump-settings.service';
import { PumpStatusService } from '../pump-status.service';
import { ConstantsService } from '../constants.service';
import {
  Subscription,
} from 'rxjs';
interface keyable {
  [key: string]: any;
}

@Component({
  selector: 'app-pump-settings',
  templateUrl: './pump-settings.component.html',
  styleUrls: ['./pump-settings.component.css']
})
export class PumpSettingsComponent implements OnInit {

  @Input()
  channel: number;

  private settingsIsReadySubscription: Subscription;
  private statusIsReadySubscription: Subscription;

  constructor(
    private http: HttpClient,
    private notifierService: NotifierService,
    private settingsService: PumpSettingsService,
    private statusService: PumpStatusService,
    private constantsService: ConstantsService
  ) {
    this.settings = {pump_activation_time: {hour: 0}};
  }

  settings: keyable;

  private status: keyable;

  ngOnInit(): void {

    console.log(`ngOnInit Pump-Settings Component`);

    if (this.settingsService.isReady) {
      this.doInitSettings();
    } else {
      this.settingsIsReadySubscription = this.settingsService.isReady$.subscribe((settingsIsReady: boolean) => {
        if (!settingsIsReady) console.error('settingsService.isReady$ emitted false');
        this.settingsIsReadySubscription.unsubscribe();
        this.doInitSettings();
      })
      this.settingsService.Init();
    }

    if (this.statusService.isReady) {
      this.doInitStatus();
    } else {
      this.statusIsReadySubscription = this.statusService.isReady$.subscribe((statusIsReady: boolean) => {
        if (!statusIsReady) console.error('statusService.isReady$ emitted false');
        this.statusIsReadySubscription.unsubscribe();
        this.doInitStatus();
      })
      this.statusService.Init();
    }

  }

  private doInitSettings(): void {
    this.settingsService.allSettings$[this.channel].subscribe(
      (data: keyable) => {
        this.settings = data.data;
      }
    )

    this.settingsService.updateSettings(this.channel);
  }

  private doInitStatus(): void {
    this.statusService.allStatuses[this.channel].subscribe((data: keyable) => {
      this.status = data.data;
    });
  }


  onTakeCurrentHumidityAsDry(): void {
    this.settings.dry_humidity_V = this.status.rel_humidity_V;
    this.onSettingsChange();
  }

  onTakeCurrentHumidityAsWet(): void {
    this.settings.wet_humidity_V = this.status.rel_humidity_V;
    this.onSettingsChange();
  }

  onRefreshSettings(): void {
    this.settingsService.updateSettings(this.channel);
  }

  onSettingsChange(): void {
    this.settingsService.setSettings(this.channel, this.settings).subscribe(
      (data: keyable) => {
        this.settings = data.data;
        this.notifierService.notify('success', `${this.channel}: Settings updated`);
        this.settingsService.updateSettings(this.channel);
      },
      (error: keyable) => {
        this.notifierService.notify('error', `setSettings Error:  ${error.message}`);
      }
    )
  }

  onSaveSettings(): void {
    this.http.get(`${this.constantsService.kBackendURL}save_settings/${this.channel}`).subscribe(
      (data: keyable) => {
        this.notifierService.notify('success', `Saved settings to pump ${this.channel}'s flash memory`);
      },
      (error: keyable) => {
        this.notifierService.notify('error', `Save Error:  ${error.message}`);
      }
    );
  }

}
