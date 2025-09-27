/*
 * Web UI Routes
 *
 * Compressed Size Summary:
 * ui_app_immutable_assets_css: 10,361 bytes
 * ui_html: 3,245 bytes
 * ui_app_immutable_entry_js: 48,592 bytes
 * ui_app_immutable_nodes_js: 514 bytes
 * ui_svg: 456 bytes
 * Total: 63,168 bytes
 */

#pragma once

#include <ESPAsyncWebServer.h>
#include "ui_app_immutable_assets_css.h"
#include "ui_html.h"
#include "ui_app_immutable_entry_js.h"
#include "ui_app_immutable_nodes_js.h"
#include "ui_svg.h"

inline void setupRoutes(AsyncWebServer* server) {
    server->on("/app/immutable/assets/internal.CNjsDNpd.css", HTTP_GET, serveAppImmutableAssetsInternalCNjsDNpdCss);
    server->on("/app/immutable/assets/start.BSuysxAz.css", HTTP_GET, serveAppImmutableAssetsStartBSuysxAzCss);
    server->on("/app/immutable/entry/app.qWItaoAN.js", HTTP_GET, serveAppImmutableEntryAppQWItaoAnJs);
    server->on("/app/immutable/entry/start.B2a8TaZU.js", HTTP_GET, serveAppImmutableEntryStartB2a8TaZuJs);
    server->on("/app/immutable/nodes/0.DzITOQVV.js", HTTP_GET, serveAppImmutableNodes_0DzItoqvvJs);
    server->on("/app/immutable/nodes/1.B-zIyyc5.js", HTTP_GET, serveAppImmutableNodes_1BZIyyc5Js);
    server->on("/app/immutable/nodes/2.B6twBuCE.js", HTTP_GET, serveAppImmutableNodes_2B6twBuCeJs);
    server->on("/app/immutable/nodes/3.BB9eiVQ4.js", HTTP_GET, serveAppImmutableNodes_3Bb9eiVq4Js);
    server->on("/app/immutable/nodes/4.svg1oD0r.js", HTTP_GET, serveAppImmutableNodes_4Svg1oD0rJs);
    server->on("/app/immutable/nodes/5.BLWt4Gt2.js", HTTP_GET, serveAppImmutableNodes_5BlWt4Gt2Js);
    server->on("/app/immutable/nodes/6.Bbp-CeoU.js", HTTP_GET, serveAppImmutableNodes_6BbpCeoUJs);
    server->on("/favicon.svg", HTTP_GET, serveFaviconSvg);
    // HTML routes
    server->on("/devices", HTTP_GET, serveDevicesHtml);
    server->on("/devices.html", HTTP_GET, serveDevicesHtml);
    server->on("/fingerprints", HTTP_GET, serveFingerprintsHtml);
    server->on("/fingerprints.html", HTTP_GET, serveFingerprintsHtml);
    server->on("/", HTTP_GET, serveIndexHtml);
    server->on("/network", HTTP_GET, serveNetworkHtml);
    server->on("/network.html", HTTP_GET, serveNetworkHtml);
    server->on("/settings", HTTP_GET, serveSettingsHtml);
    server->on("/settings.html", HTTP_GET, serveSettingsHtml);
}
