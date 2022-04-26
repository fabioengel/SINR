//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package exemplo01;
import exemplo01.src.device.Host;
import inet.environment.common.PhysicalEnvironment;
import inet.node.inet.INetworkNode;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.INetworkNode;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.visualizer.common.IntegratedVisualizer;

network Network
{
    parameters:
        int simId = default(-1);
        int numHosts = default(6);
        bool startPosition = startPositionGeneration(numHosts,simId);


        @display("bgb=588,182");
    submodules:
        host[numHosts]: Host {
            myId = index;
            @display("p=21,29");
        }

        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer {
            @display("p=454,138");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=543,138");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=348,137");
        }
        physicalEnvironment: PhysicalEnvironment {
            @display("p=217,138");
        }
}
