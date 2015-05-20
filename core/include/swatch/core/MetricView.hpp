/**
 * @file    MetricView.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_METRICVIEW_HPP__
#define	__SWATCH_CORE_METRICVIEW_HPP__


#include <map>
#include <string>


namespace swatch {
namespace core {

class AbstractMetric;
class MonitorableObject;

//! Represents map of Metrics whose ID paths match a specified regex
class MetricView{
public:
    
    MetricView(swatch::core::MonitorableObject& aMonObject, const std::string& aRegex);
    
    ~MetricView();
    
    //! Call update method of all metrics in this view
    void update();
    
    //! Number of metrics in the view
    size_t size() const;
    
    typedef std::map<std::string, swatch::core::AbstractMetric*>::iterator iterator;

    typedef std::map<std::string, swatch::core::AbstractMetric*>::const_iterator const_iterator;

    iterator begin();
    
    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
    
    
private:
    
    typedef std::map<std::string, swatch::core::AbstractMetric*> MetricMap;
    
    std::map<std::string, swatch::core::AbstractMetric*> metricsMap_;
};

} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_METRICVIEW_HPP__ */

