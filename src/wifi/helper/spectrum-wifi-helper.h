/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef SPECTRUM_WIFI_HELPER_H
#define SPECTRUM_WIFI_HELPER_H

#include "wifi-helper.h"

namespace ns3
{

class SpectrumChannel;

/**
 * \brief Make it easy to create and manage PHY objects for the spectrum model.
 *
 * The Pcap and ASCII traces generated by the EnableAscii and EnablePcap methods defined
 * in this class correspond to PHY-level traces and come to us via WifiPhyHelper
 *
 */
class SpectrumWifiPhyHelper : public WifiPhyHelper
{
  public:
    /**
     * Create a PHY helper.
     *
     * \param nLinks the number of links to configure (>1 only for 11be devices)
     */
    SpectrumWifiPhyHelper(uint8_t nLinks = 1);

    /**
     * \param channel the default spectrum channel to associate to this helper
     *
     * Every PHY created by a call to Install is associated to this default spectrum channel.
     */
    void SetChannel(const Ptr<SpectrumChannel> channel);
    /**
     * \param channelName The name of the default spectrum channel to associate to this helper
     *
     * Every PHY created by a call to Install is associated to this default spectrum channel.
     */
    void SetChannel(const std::string& channelName);

    /**
     * \param channel the spectrum channel to add to this helper
     * \param freqRange the frequency range, bounded by a minFrequency and a maxFrequency in MHz
     *
     * Every PHY created by a call to Install is added to this spectrum channel.
     * If a PHY is requested to operate or scan a channel with the specified frequency and width
     * combination, it will activate that channel and deactivate the current channel for that PHY.
     */
    void AddChannel(const Ptr<SpectrumChannel> channel,
                    const FrequencyRange& freqRange = WHOLE_WIFI_SPECTRUM);
    /**
     * \param channelName The name of the spectrum channel to add to this helper
     * \param freqRange the frequency range, bounded by a minFrequency and a maxFrequency in MHz
     *
     * Every PHY created by a call to Install is added to this spectrum channel.
     * If a PHY is requested to operate or scan a channel with the specified frequency and width
     * combination, it will activate that channel and deactivate the current channel for that PHY.
     */
    void AddChannel(const std::string& channelName,
                    const FrequencyRange& freqRange = WHOLE_WIFI_SPECTRUM);

  private:
    /**
     * \param node the node on which we wish to create a wifi PHY
     * \param device the device within which this PHY will be created
     * \returns newly-created PHY objects.
     *
     * This method implements the pure virtual method defined in \ref ns3::WifiPhyHelper.
     */
    std::vector<Ptr<WifiPhy>> Create(Ptr<Node> node, Ptr<WifiNetDevice> device) const override;

    std::map<FrequencyRange, Ptr<SpectrumChannel>> m_channels; ///< the spectrum channels
};

} // namespace ns3

#endif /* SPECTRUM_WIFI_HELPER_H */
